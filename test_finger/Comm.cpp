#include "stdafx.h"

HANDLE dataQueueMutex=CreateMutex(0,0,0);
std::queue<DataPacket> dataQueue;
BYTE buffer[1<<18];
Comm comm;

Comm::Comm(){
    id=0;
    type=0xEF02;
    listenThread=0;
    attach(new RequestConverterACH512());
    attach(new RequestConverterGD32F30());
    attach(new ResponseConverterGD32F30());
}

uint16_t Comm::getType(){
    return type;
}

int Comm::getConnectId(){
    return id;
}

DWORD WINAPI ResponseThread(LPVOID params){
    auto responseVector=(std::vector<ICommProtocolResponseConverter*>*)params;
    DataPacket dataPacket;
    while(1){
        WaitForSingleObject(dataQueueMutex,INFINITE);
        if(dataQueue.size()){
            dataPacket=dataQueue.front();
            dataQueue.pop();
        }
        ReleaseMutex(dataQueueMutex);
        if(dataPacket.isValid()){
            uint16_t head=dataPacket.data[0]*256+dataPacket.data[1];
            for(auto it=responseVector->begin();it!=responseVector->end();it++){
                if((*it)->checkProtocol(head)){
                    auto data=(*it)->convert(dataPacket);
                    int cmdCode=(*it)->getCmdCode(dataPacket);
                    boardcastListener.boardcast(cmdCode,data);
                    data.Destruction();
                }
            }
            dataPacket.Destruction();
        }
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
        serial.Purge();
    }
    MyLog.print(Log::LOGU,"连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
    if(retCode==ERROR_SUCCESS){
        responseThread=CreateThread(0,0,ResponseThread,&responseVector,0,0);
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
    MyLog.print(Log::LOGU,"断开连接成功");
    if(ret==ERROR_SUCCESS){
        id=0;
        return true;
    } else{
        ASF_WARNING(5);
        return false;
    }
}

void Comm::request(int CmdCode,uint8_t * Data,uint16_t Len){
    for(auto it=requestVector.begin();it!=requestVector.end();it++){
        if((*it)->checkProtocol(getType())){
            auto dataPacket=(*it)->convert(CmdCode,Data,Len);
            std::vector<DataPacket>::iterator it;
            for(it=dataPacket.begin();it!=dataPacket.end();it++){
                serial.Write(it->data,it->len);
                it->Destruction();
            }
            break;
        }
    }
}

void Comm::attach(ICommProtocolRequestConverter * converter){
    requestVector.push_back(converter);
}

void Comm::attach(ICommProtocolResponseConverter * converter){
    responseVector.push_back(converter);
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
