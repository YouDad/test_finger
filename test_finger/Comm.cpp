#include "stdafx.h"

MyLocker dataQueueMutex;
//指示lastCmdCode的CmdCode的数量
MyLocker vaildLastCmdCode(0,10);
std::queue<DataPacket> dataQueue;
uint8_t buffer[1<<16];
Comm comm;
int CommType=-1;
bool USB_Send(BYTE CmdBuf[],DWORD CmdLength);
bool USB_Receive(BYTE RspBuf[],DWORD RspLength,ULONG& RspReadLen);

Comm::Comm(){
    id=0;
    listenThread=0;
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
        retCode=serial.Open(way,2048,2048,true);
    }
    //其余三参数均采用默认参数
    if(retCode==ERROR_SUCCESS){
        retCode=serial.Setup((CSerial::EBaudrate)baud);
    }
    if(retCode==ERROR_SUCCESS){
        retCode=serial.SetMask(CSerial::EEventRecv);
    }
    if(retCode==ERROR_SUCCESS){
        retCode=serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);
    }
    if(retCode==ERROR_SUCCESS){
        serial.Purge();
    }
    MyLog::user("连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
    if(retCode==ERROR_SUCCESS){
        responseThread=CreateThread(0,0,ResponseThread,0,0,0);
        startListen();
        this->id=id;
    }
    return retCode==ERROR_SUCCESS;
}

bool Comm::connectUSB(){
    if(-1!=UsbPort.InitUsbPort(COMM_USB_MASS,"UD")){
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
        ret=UsbPort.CloseUsbPort();
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
    static MyThread* Comm_request=nullptr;
    static uint8_t* data=nullptr;
    static uint16_t len;
    static int cmdCode;
    if(Comm_request){
        Comm_request->terminate();
        delete Comm_request;
    }
    if(data){
        delete[] data;
    }
    cmdCode=CmdCode;
    len=Len;
    data=new uint8_t[len];
    memcpy(data,Data,len);
    Comm_request=new MyThread([&](){
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
    });
    Comm_request->start();
}

DWORD WINAPI ListenSerialThread(LPVOID params){
    CSerial* serial=(CSerial*)params;
    while(serial->IsOpen()){
        DWORD cnt;
        LONG result;
        result=serial->Read(buffer,sizeof(buffer),&cnt);
        if(result==ERROR_SUCCESS){
            dataQueue.push(DataPacket(buffer,cnt));
            dataQueueMutex.unlock();
        }
    }
    return 0;
}

DWORD WINAPI ListenUSBThread(LPVOID params){
    while(-1!=*(int*)params){
        ULONG cnt;
        if(USB_Receive(buffer,sizeof(buffer),cnt)){
            dataQueue.push(DataPacket(buffer,sizeof(buffer)));
            dataQueueMutex.unlock();
        }
    }
    return 0;
}

bool Comm::startListen(){
    if(listenThread==0){
        if(CommType==-1){
            listenThread=CreateThread(0,0,ListenSerialThread,&serial,0,0);
        } else{
            listenThread=CreateThread(0,0,ListenUSBThread,&CommType,0,0);
        }
    }
    return listenThread!=0;
}

bool Comm::terminateListen(){
    if(listenThread){
        bool ret=TerminateThread(listenThread,-1);
        CloseHandle(listenThread);
        listenThread=0;
        return ret;
    }
    return true;
}

void Comm::sendBytes(uint8_t * data,int len){
    if(CommType==-1){
        serial.Write(data,len);
    } else{
        USB_Send(data,len);
    }
}

BYTE cdb_w[16]={0xef,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BYTE cdb_r[16]={0xef,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

bool USB_Send(BYTE CmdBuf[],DWORD CmdLength){
    return UsbPort.USBSCSIWrite(cdb_w,IOCTRL_CDB_LEN,CmdBuf,CmdLength,SCSI_TIMEOUT);
}

bool USB_Receive(BYTE RspBuf[],DWORD RspLength,ULONG& RspReadLen){
    return UsbPort.USBSCSIRead(cdb_r,IOCTRL_CDB_LEN,RspBuf,RspLength,RspReadLen,SCSI_TIMEOUT);
}
