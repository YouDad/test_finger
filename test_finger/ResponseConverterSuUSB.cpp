#include "stdafx.h"
typedef DataPacketSuUSB Response;

bool ResponseConverterSuUSB::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    char* head=(char*)dataPacket.getPointer();
    return MyString(head,sizeof(Response::head))=="zt";
}

DataPacket ResponseConverterSuUSB::convert(DataPacket & data){
    return DataPacket();
}

int ResponseConverterSuUSB::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->cmd;
}
