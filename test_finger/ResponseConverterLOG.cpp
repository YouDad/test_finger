#include "stdafx.h"
#include "ResponseConverterLOG.h"
typedef DataPacketLOG Response;

bool ResponseConverterLOG::checkProtocol(uint16_t head){
    return head=='L'*256+'O';
}

DataPacket ResponseConverterLOG::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int&read=data.read;
    int len=data.len-read;
    BYTE* p=data.data+read;
    int totalLength=0;
    BYTE* tmpArray=new BYTE[len];
    Response* pData=(Response*)p;
    while(len&&MyString("LOG")==MyString(pData->Head,3)){
        memcpy(tmpArray+totalLength,pData->Data,pData->Len);
        totalLength+=pData->Len;
        p+=size+pData->Len;
        read+=size+pData->Len;
        len-=size+pData->Len;
        pData=(Response*)p;
    }
    DataPacket ret(tmpArray,totalLength);
    delete[] tmpArray;
    return ret;
}

int ResponseConverterLOG::getCmdCode(DataPacket data){
    return ((Response*)(data.data+data.read))->Cmd;
}
