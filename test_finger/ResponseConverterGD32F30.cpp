#include "stdafx.h"
#include "ResponseConverterGD32F30.h"
typedef ResponsePacketGD32F30 Response;

bool ResponseConverterGD32F30::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    BYTE*p=dataPacket.getPointer();
    return p[0]==0xEF&&p[1]==0x02;
}

DataPacket ResponseConverterGD32F30::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int tmpLength=0;
    BYTE* tmpArray=data.getTempArray();
    Response* pData=(Response*)data.getPointer();
    while(checkProtocol(data)){
        memcpy(tmpArray+tmpLength,pData->Sendbuf,pData->Length);
        tmpLength+=pData->Length;
        data.readData(size+pData->Length);
        pData=(Response*)data.getPointer();
    }
    DataPacket ret(tmpArray,tmpLength);
    delete[] tmpArray;
    return ret;
}

int ResponseConverterGD32F30::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->CMD;
}
