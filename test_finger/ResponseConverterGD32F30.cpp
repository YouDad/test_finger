#include "stdafx.h"

bool ResponseConverterGD32F30::checkProtocol(uint16_t head){
    return head==0xEF02;
}

DataPacket ResponseConverterGD32F30::convert(DataPacket data){
    const int size=sizeof DataPacketGD32F30;
    int len=data.len;
    void* p=data.data;
    int totalLength=0;
    BYTE* tmpArray=new BYTE[len];
    while(len){
        DataPacketGD32F30* pData=(DataPacketGD32F30*)p;
        memcpy(tmpArray+totalLength,pData->Sendbuf,pData->Length);
        totalLength+=pData->Length;
        p=(BYTE*)p+min(len,size);
        len-=min(len,size);
    }
    DataPacket ret(tmpArray,totalLength);
    delete[] tmpArray;
    return ret;
}

int ResponseConverterGD32F30::getCmdCode(DataPacket data){
    int cmdCode=((DataPacketGD32F30*)data.data)->CMD;
    return cmdCode;
}
