#include "stdafx.h"
typedef DataPacketLOG Response;

bool ResponseConverterLOG::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    uint8_t* p=dataPacket.getPointer();
    return p[0]=='L'&&p[1]=='O'&&p[2]=='G';
}

DataPacket ResponseConverterLOG::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int totalLength=0;
    uint8_t* tmpArray=new uint8_t[data.readSize()];
    auto pData=(Response*)data.getPointer();
    while(checkProtocol(data)){
        memcpy(tmpArray+totalLength,pData->Data,pData->Len);
        totalLength+=pData->Len;
        data.readData(size+pData->Len);
        if(pData->Sign==DataEnd){
            break;
        }
        pData=(Response*)data.getPointer();
    }
    DataPacket ret(tmpArray,totalLength);
    delete[] tmpArray;
    return ret;
}

int ResponseConverterLOG::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->Cmd;
}
