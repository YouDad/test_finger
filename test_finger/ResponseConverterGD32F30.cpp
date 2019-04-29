#include "stdafx.h"
#include "ResponseConverterGD32F30.h"
typedef ResponsePacketGD32F30 Response;

bool ResponseConverterGD32F30::checkProtocol(uint16_t head){
    return head==0xEF02;
}

DataPacket ResponseConverterGD32F30::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int&read=data.read;
    int len=data.len-read;
    BYTE* p=data.data+read;
    int totalLength=0;
    BYTE* tmpArray=new BYTE[len];
    Response* pData=(Response*)p;
    while(len&&pData->Head==0x02EF){
        memcpy(tmpArray+totalLength,pData->Sendbuf,pData->Length);
        totalLength+=pData->Length;
        len-=size+pData->Length;
        read+=size+pData->Length;
        p+=size+pData->Length;
        pData=(Response*)p;
    }
    DataPacket ret(tmpArray,totalLength);
    delete[] tmpArray;
    return ret;
}

int ResponseConverterGD32F30::getCmdCode(DataPacket data){
    return ((Response*)(data.data+data.read))->CMD;
}
