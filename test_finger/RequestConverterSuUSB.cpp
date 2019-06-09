#include "stdafx.h"
typedef DataPacketSuUSB Request;

bool RequestConverterSuUSB::checkProtocol(DataPacket dataPacket){
    return getText(cmbProtocolType)=="SuUSB";
}

int ToSuUSB(int cmdCode){
    switch(cmdCode){
    case SII(DeviceInfo):
        return __SCC(SuUSB,DeviceInfo);
    default:
        ASF_ERROR(6);
        throw 0;
    }
}

std::vector<DataPacket> RequestConverterSuUSB::convert(int CmdCode,uint8_t * Data,uint16_t Len){
    std::vector<DataPacket> ret;
    try{
        CmdCode=ToSuUSB(CmdCode);
    } catch(...){
        return ret;
    }

    Request request={};
    request.result=0;
    request.head[0]='z';
    request.head[1]='t';
    request.dir=DIR_OUT;
    request.cmd=CmdCode;
    request.len=Len;
    memcpy(request.data,Data,Len);

    uint16_t crc=0;
    for(int i=2;i<sizeof request-sizeof request.data+Len;i++){
        crc+=((uint8_t*)&request)[i];
    }
    memcpy(request.data+Len,&crc,2);

    ret.push_back(DataPacket(&request,sizeof request-sizeof request.data+Len+2));
    return ret;
}
