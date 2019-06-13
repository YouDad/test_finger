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
    responseThread=new MyThread(
        [&](){
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
    );
}

int Comm::getConnectId(){
    return this->id;
}

bool Comm::connect(int id,int baud){
    CString way;
    way.Format(_T("\\\\.\\COM%d"),id);//得到端口地址
    LONG retCode=ERROR_SUCCESS;

    if(retCode==ERROR_SUCCESS){ retCode=this->serial.Open(way,2048,2048,true); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.Setup((CSerial::EBaudrate)baud); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.SetMask(CSerial::EEventRecv); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking); }
    if(retCode==ERROR_SUCCESS){ this->serial.Purge(); }
    MyLog::user("连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
    if(retCode==ERROR_SUCCESS){
        this->responseThread->start();
        this->listenThread->start();
        this->id=id;
    }
    return retCode==ERROR_SUCCESS;
}

bool Comm::connectUSB(){
    if(-1!=this->usb.InitUsbPort(COMM_USB_MASS,"UD")){
        MyLog::user("连接USB成功");
        CommType=COMM_USB_MASS;
        this->responseThread->start();
        this->listenThread->start();
        return true;
    } else{
        MyLog::user("连接USB失败(注意,连接USB需要管理员权限)");
        CommType=-1;
        return false;
    }

}

bool Comm::disconnect(){
    this->responseThread->terminate();
    this->listenThread->terminate();

    int ret;
    if(CommType==-1){
        ret=this->serial.Close();
    } else{
        ret=this->usb.CloseUsbPort();
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

void Comm::request(int cmdCode,DataPacket packet){
    (new MyThread(
        [=](){
            auto converter=converterBoardcast.RequestConvert();
            if(converter){
                auto dataPacket=converter->convert(cmdCode,packet.getPointer(),packet.size());
                for(auto it=dataPacket.begin();it!=dataPacket.end();it++){
                    this->sendBytes(it->getPointer(),it->size());
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
        return this->serial.IsOpen();
    } else if(id==-1){
        return CommType!=-1;
    } else{
        return false;
    }
}

BYTE cdb_w[16]={0xef,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void Comm::sendBytes(uint8_t * data,int len){
    if(CommType==-1){
        this->serial.Write(data,len);
    } else{
        this->usb.USBSCSIWrite(cdb_w,IOCTRL_CDB_LEN,data,len,SCSI_TIMEOUT);
    }
}

BYTE cdb_r[16]={0xef,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
bool Comm::readBytes(uint8_t* buffer,int blen,ULONG& cnt){
    if(CommType==-1){
        return this->serial.Read(buffer,blen,&cnt)==ERROR_SUCCESS;
    } else{
        return this->usb.USBSCSIRead(cdb_r,IOCTRL_CDB_LEN,buffer,blen,cnt,SCSI_TIMEOUT);
    }
}
