#include "stdafx.h"
typedef DataPacketASFComm Response;

bool ResponseConverterASFComm::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    char* head=(char*)dataPacket.getPointer();
    return MyString(head,sizeof(Response::head))=="ASF";
}

DataPacket ResponseConverterASFComm::convert(DataPacket& data){
    Response* response=(Response*)data.getPointer();
    BYTE* arr=data.getNewArray();
    const int header=sizeof(Response)-sizeof(response->data);
    int len=0;
    while(checkProtocol(data)){
        memcpy(arr+len,response->data,response->len);
        len+=response->len;
        data.readData(header+response->len);
        if(response->sign==DataEnd){
            break;
        }
        response=(Response*)data.getPointer();
    }
    DataPacket ret(arr,len);
    delete[] arr;
    return ret;
}

int ResponseConverterASFComm::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->cmd;
}
