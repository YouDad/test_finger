#pragma once
#include"stdafx.h"

#pragma pack(1)
struct DataPacketGD32F30{
    uint16_t	Head;
    uint32_t	Addr;
    uint32_t	Password;
    uint16_t	NO;
    uint16_t	CMD;
    uint8_t		Sign;
    uint16_t	Length;
    uint8_t		Sendbuf[530];
};
#pragma pack(4)

class RequestConverterGD32F30:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};
