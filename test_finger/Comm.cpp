#include "stdafx.h"

HANDLE dataQueueMutex=CreateMutex(0,0,0);
HANDLE lastCmdCodeMutex=CreateMutex(0,0,0);
std::queue<DataPacket> dataQueue;
uint8_t buffer[1<<18];
Comm comm;

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
        WaitForSingleObject(dataQueueMutex,INFINITE);
        if(dataQueue.size()){
            dataPacket=dataQueue.front();
            dataQueue.pop();
        }
        ReleaseMutex(dataQueueMutex);
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

bool Comm::disconnect(){
    TerminateThread(responseThread,-1);
    CloseHandle(responseThread);
    responseThread=0;
    terminateListen();

    int ret=serial.Close();
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
    Comm_request=new MyThread(ThreadFunction__(request)(void){
        auto converter=converterBoardcast.RequestConvert();
        if(converter){
            auto dataPacket=converter->convert(cmdCode,data,len);
            for(auto it=dataPacket.begin();it!=dataPacket.end();it++){
                serial.Write(it->getPointer(),it->size());
                it->Destruction();
            }
        } else{
            ASF_ERROR(3);
        }
    });
    Comm_request->start();
}

DWORD WINAPI ListenThread(LPVOID params){
    CSerial* serial=(CSerial*)params;
    while(serial->IsOpen()){
        DWORD cnt;
        LONG result;
        result=serial->Read(buffer,1<<18,&cnt);
        if(result==ERROR_SUCCESS){
            WaitForSingleObject(dataQueueMutex,INFINITE);
            dataQueue.push(DataPacket(buffer,cnt));
            ReleaseMutex(dataQueueMutex);
        }
    }
    return 0;
}

bool Comm::startListen(){
    if(listenThread==0){
        listenThread=CreateThread(0,0,ListenThread,&serial,0,0);
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
