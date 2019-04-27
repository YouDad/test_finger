#include "stdafx.h"
#include "RequestConverterGD32F30.h"

bool RequestConverterGD32F30::checkProtocol(uint16_t head){
    return head==0xEF02;
}

std::vector<DataPacket> RequestConverterGD32F30::convert(CmdCodeGD32F30 CmdCode,uint8_t * Data,uint16_t Len){
    std::vector<DataPacket> ret;

    struct RequestPacketGD32F30 SendPack={};
    SendPack.Head=0x02EF;
    SendPack.CMD=CmdCode;

    const int interval=sizeof RequestPacketGD32F30::Sendbuf-2;
    for(int i=0;Len>interval+i*interval;i++){
        SendPack.NO=i;
        SendPack.Sign=RequestNotEnd;
        SendPack.Length=interval;
        memcpy(SendPack.Sendbuf,Data+SendPack.NO*interval,interval);
        UINT16 crcValue=GetCRC16(&SendPack,sizeof RequestPacketGD32F30-interval+SendPack.Length);
        SendPack.Sendbuf[interval]=crcValue&0xFF;
        SendPack.Sendbuf[interval+1]=(crcValue>>8)&0xFF;
        ret.push_back(DataPacket(&SendPack,sizeof SendPack));
    }
    SendPack.NO=Len/interval;
    SendPack.Sign=RequestEnd;
    if(Len>0&&Len%interval==0){
        SendPack.Length=interval;
    } else{
        SendPack.Length=Len%interval;
    }
    memcpy(SendPack.Sendbuf,Data+SendPack.NO*interval,interval);
    UINT16 crcValue=GetCRC16(&SendPack,sizeof RequestPacketGD32F30-interval+SendPack.Length);
    SendPack.Sendbuf[interval]=crcValue&0xFF;
    SendPack.Sendbuf[interval+1]=(crcValue>>8)&0xFF;
    ret.push_back(DataPacket(&SendPack,sizeof SendPack));
    return ret;
}