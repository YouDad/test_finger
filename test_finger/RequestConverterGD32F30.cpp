#include "stdafx.h"

bool RequestConverterGD32F30::checkProtocol(DataPacket dataPacket){
    return getText(cmbProtocolType)=="GD32F30";
}

std::vector<DataPacket> RequestConverterGD32F30::convert(int CmdCode,uint8_t * Data,uint16_t Len){
    std::vector<DataPacket> ret;

    struct DataPacketGD32F30 SendPack={};
    SendPack.Head=0x02EF;
    SendPack.CMD=CmdCode;

    const int interval=sizeof DataPacketGD32F30::Sendbuf-2;
    for(int i=0;Len>interval+i*interval;i++){
        SendPack.NO=i;
        SendPack.Sign=DataNotEnd;
        SendPack.Length=interval;
        memset(SendPack.Sendbuf,0,sizeof SendPack.Sendbuf);
        if(Data){
            memcpy(SendPack.Sendbuf,Data+SendPack.NO*interval,interval);
        }
        UINT16 crcValue=GetCRC16(&SendPack,sizeof DataPacketGD32F30-interval+SendPack.Length);
        SendPack.Sendbuf[SendPack.Length]=crcValue&0xFF;
        SendPack.Sendbuf[SendPack.Length+1]=(crcValue>>8)&0xFF;
        ret.push_back(DataPacket(&SendPack,sizeof SendPack));
    }
    SendPack.NO=Len/interval;
    SendPack.Sign=DataEnd;
    if(Len>0&&Len%interval==0){
        SendPack.Length=interval;
    } else{
        SendPack.Length=Len%interval;
    }
    memset(SendPack.Sendbuf,0,sizeof SendPack.Sendbuf);
    if(Data){
        memcpy(SendPack.Sendbuf,Data+SendPack.NO*interval,interval);
    }
    UINT16 crcValue=GetCRC16(&SendPack,sizeof DataPacketGD32F30-interval+SendPack.Length);
    SendPack.Sendbuf[SendPack.Length]=crcValue&0xFF;
    SendPack.Sendbuf[SendPack.Length+1]=(crcValue>>8)&0xFF;
    ret.push_back(DataPacket(&SendPack,sizeof SendPack-interval+SendPack.Length));
    return ret;
}
