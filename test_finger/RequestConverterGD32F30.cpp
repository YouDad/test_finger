#include "stdafx.h"
typedef struct DataPacketGD32F30 Request;

bool RequestConverterGD32F30::checkProtocol(DataPacket dataPacket){
    return getText(cmbProtocolType)=="GD32F30";
}

int ToGD32F30(int cmdCode){
    switch(cmdCode){
        case SII(GetTestImage):
            return CMD_GET_TEST_IMAGE;
        case SII(GetRawImage):
            return CMD_GET_RAW_IMAGE;
        case SII(ReadRegister):
            return CMD_READ_NOTE_BOOK;
        case SII(WriteRegister):
            return CMD_WRITE_NOTE_BOOK;
        case SII(DeviceInfo):
            return __SCC(GD32F30,DeviceInfo);
        default:
            ASF_ERROR(6);
            throw 0;
    }
}

std::vector<DataPacket> RequestConverterGD32F30::convert(int CmdCode,uint8_t * Data,uint16_t Len){
    std::vector<DataPacket> ret;
    try{
        CmdCode=ToGD32F30(CmdCode);
    } catch(...){
        return ret;
    }

    Request request={};
    request.Head=0x02EF;
    request.CMD=CmdCode;

    const int interval=sizeof request.Sendbuf-2;
    for(int i=0;Len>interval+i*interval;i++){
        request.NO=i;
        request.Sign=DataNotEnd;
        request.Length=interval;
        memset(request.Sendbuf,0,sizeof request.Sendbuf);
        if(Data){
            memcpy(request.Sendbuf,Data+request.NO*interval,interval);
        }
        UINT16 crcValue=GetCRC16(&request,sizeof Request-interval+request.Length);
        request.Sendbuf[request.Length]=crcValue&0xFF;
        request.Sendbuf[request.Length+1]=(crcValue>>8)&0xFF;
        ret.push_back(DataPacket(&request,sizeof request));
    }
    request.NO=Len/interval;
    request.Sign=DataEnd;
    if(Len>0&&Len%interval==0){
        request.Length=interval;
    } else{
        request.Length=Len%interval;
    }
    memset(request.Sendbuf,0,sizeof request.Sendbuf);
    if(Data){
        memcpy(request.Sendbuf,Data+request.NO*interval,interval);
    }
    UINT16 crcValue=GetCRC16(&request,sizeof Request-interval+request.Length);
    request.Sendbuf[request.Length]=crcValue&0xFF;
    request.Sendbuf[request.Length+1]=(crcValue>>8)&0xFF;
    ret.push_back(DataPacket(&request,sizeof request-interval+request.Length));
    return ret;
}
