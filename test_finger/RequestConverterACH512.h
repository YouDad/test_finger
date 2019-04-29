#pragma once
#include"stdafx.h"

#pragma pack(1)
struct HangXinSendPacket{
    char fixed[3];//'z','t','\0'
    uint8_t cmd;
    char fixed2[4];//'\0','\0','\0','\0'
    uint32_t len;
    uint8_t data[242*266+2];
    uint16_t sumVal;
};
#pragma pack(4)

class RequestConverterACH512:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};
