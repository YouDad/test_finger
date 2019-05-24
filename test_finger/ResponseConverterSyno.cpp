#include "stdafx.h"
typedef DataPacketSyno Response;

bool ResponseConverterSyno::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    unsigned char* head=dataPacket.getPointer();
    return head[0]==0xEF&&head[1]==0x01;
}

DataPacket ResponseConverterSyno::convert(DataPacket& data){
    Response response;
    memcpy(&response,data.getPointer(),sizeof(response));
    BYTE* arr=new BYTE[data.readSize()];
    const int header=sizeof(Response)-sizeof(response.data)-sizeof(response.sum);
    int len=0;
    while(checkProtocol(data)){
        response.reconvert();
        memcpy(arr+len,response.data,response.len);
        len+=response.len;
        data.readData(header+response.len);
        //TODO У��λ
        data.readData(2);
        if(response.sign==SynoSign::EndData){
            break;
        }
        memcpy(&response,data.getPointer(),sizeof(response));
    }
    DataPacket ret(arr,len);
    delete[] arr;
    return ret;
}

int ResponseConverterSyno::getCmdCode(DataPacket data){
    int ret;
    WaitForSingleObject(lastCmdCodeMutex,INFINITE);
    if(lastCmdCode.size()==1){
        ret=lastCmdCode.front();
        lastCmdCode.pop();
    } else{
        ret=-1;
    }
    ReleaseMutex(lastCmdCodeMutex);
    return ret;
}