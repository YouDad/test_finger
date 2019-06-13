#include "stdafx.h"

MyLocker dataQueueMutex;
//指示lastCmdCode的CmdCode的数量
MyLocker vaildLastCmdCode(0,10);
std::queue<DataPacket> dataQueue;
uint8_t buffer[1<<16];
Comm comm;
int CommType=-1;

Comm::Comm(){
    id=0;
    listenThread=new MyThread(
        [&](){
            while(this->isOpen()){
                ULONG cnt;
                if(readBytes(buffer,sizeof buffer,cnt)){
                    cnt=max(cnt,sizeof buffer);
                    dataQueue.push(DataPacket(buffer,cnt));
                    dataQueueMutex.unlock();
                }
            }
            return 0;
        }
    );
}

int Comm::getConnectId(){
    return id;
}

DWORD WINAPI ResponseThread(LPVOID params){
    DataPacket dataPacket;
    while(1){
        dataQueueMutex.lock();
        if(dataQueue.size()){
            dataPacket=dataQueue.front();
            dataQueue.pop();
        }
        while(dataPacket.isValid()){
            int cmdCode=converterBoardcast.ResponseGetCmdCode(dataPacket);
            //没有协议可以转化剩下的数据了,数据废了
            if(cmdCode==-1){
                break;
            }
            auto data=converterBoardcast.ResponseConvert(dataPacket);
            listenerBoardcast.boardcast(cmdCode,data);
            data.Destruction();
        }
        dataPacket.Destruction();
    }
}

bool Comm::connect(int id,int baud){
    CString way;
    way.Format(_T("\\\\.\\COM%d"),id);//得到端口地址
    LONG retCode=ERROR_SUCCESS;

    if(retCode==ERROR_SUCCESS){
        retCode=this->serial.Open(way,2048,2048,true);
    }
    //其余三参数均采用默认参数
    if(retCode==ERROR_SUCCESS){
        retCode=this->serial.Setup((CSerial::EBaudrate)baud);
    }
    if(retCode==ERROR_SUCCESS){
        retCode=this->serial.SetMask(CSerial::EEventRecv);
    }
    if(retCode==ERROR_SUCCESS){
        retCode=this->serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
    }
    if(retCode==ERROR_SUCCESS){
        this->serial.Purge();
    }
    MyLog::user("连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
    if(retCode==ERROR_SUCCESS){
        this->responseThread=CreateThread(0,0,ResponseThread,0,0,0);
        this->startListen();
        this->id=id;
    }
    return retCode==ERROR_SUCCESS;
}

bool Comm::connectUSB(){
    if(-1!=usb.InitUsbPort(COMM_USB_MASS,"UD")){
        MyLog::user("连接USB成功");
        CommType=COMM_USB_MASS;
        responseThread=CreateThread(0,0,ResponseThread,0,0,0);
        startListen();
        return true;
    } else{
        MyLog::user("连接USB失败(注意,连接USB需要管理员权限)");
        CommType=-1;
        return false;
    }

}

bool Comm::disconnect(){
    TerminateThread(responseThread,-1);
    CloseHandle(responseThread);
    responseThread=0;
    terminateListen();

    int ret;
    if(CommType==-1){
        ret=serial.Close();
    } else{
        ret=usb.CloseUsbPort();
    }
    MyLog::user("断开连接成功");
    if(ret==ERROR_SUCCESS){
        id=0;
        return true;
    } else{
        ASF_WARNING(5);
        return false;
    }
}

void Comm::request(int CmdCode,uint8_t * Data,uint16_t Len){
    static uint8_t* data=nullptr;
    static uint16_t len;
    static int cmdCode;
    if(data){
        delete[] data;
    }
    cmdCode=CmdCode;
    len=Len;
    data=new uint8_t[len];
    memcpy(data,Data,len);
    (new MyThread(
        [&](){
            auto converter=converterBoardcast.RequestConvert();
            if(converter){
                auto dataPacket=converter->convert(cmdCode,data,len);
                for(auto it=dataPacket.begin();it!=dataPacket.end();it++){
                    sendBytes(it->getPointer(),it->size());
                    it->Destruction();
                }
            } else{
                ASF_ERROR(3);
            }
        },true
    ))->start();
}

bool Comm::isOpen(){
    if(id>0){
        return serial.IsOpen();
    } else if(id==-1){
        return CommType!=-1;
    } else{
        return false;
    }
}

bool Comm::startListen(){
    if(!listenThread->isRun()){
        return listenThread->start();
    }
    return true;
}

bool Comm::terminateListen(){
    if(listenThread->isRun()){
        return listenThread->terminate();
    }
    return true;
}

BYTE cdb_w[16]={0xef,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void Comm::sendBytes(uint8_t * data,int len){
    if(CommType==-1){
        serial.Write(data,len);
    } else{
        usb.USBSCSIWrite(cdb_w,IOCTRL_CDB_LEN,data,len,SCSI_TIMEOUT);
    }
}

BYTE cdb_r[16]={0xef,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
bool Comm::readBytes(uint8_t* buffer,int blen,ULONG& cnt){
    if(CommType==-1){
        return serial.Read(buffer,blen,&cnt)==ERROR_SUCCESS;
    } else{
        return usb.USBSCSIRead(cdb_r,IOCTRL_CDB_LEN,buffer,blen,cnt,SCSI_TIMEOUT);
    }
}
