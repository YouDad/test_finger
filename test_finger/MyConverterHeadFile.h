#pragma once
#include"stdafx.h"

#pragma pack(1)
struct DataPacketASFComm{
    char head[3];//ASF
    unsigned short cmd;
    unsigned short len;
    unsigned char sign;
    unsigned char data[256-8];
};
#pragma pack(4)

class RequestConverterASFComm:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};

class ResponseConverterASFComm:public ICommProtocolResponseConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket & data) override;
    virtual int getCmdCode(DataPacket data) override;
};

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

#pragma pack(1)
struct ResponsePacketGD32F30{
    uint16_t	Head;
    uint32_t	Addr;
    uint32_t	Password;
    uint16_t	NO;
    uint16_t	CMD;
    uint8_t		Sign;
    uint16_t	Length;
    uint16_t    ACK;
    uint8_t		Sendbuf[530];
    static const int Header=19;
    static const int Checker=2;
};
#pragma pack(4)

class ResponseConverterGD32F30:public ICommProtocolResponseConverter{
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket& data) override;
    virtual int getCmdCode(DataPacket data) override;
};

#pragma pack(1)
struct DataPacketLOG{
    char Head[3];
    uint16_t Cmd;
    uint16_t Len;
    uint8_t Sign;
    uint8_t Data[93];
    static const int Header=8;
    static const int Checker=0;
};
#pragma pack(4)

class ResponseConverterLOG:public ICommProtocolResponseConverter{
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket& data) override;
    virtual int getCmdCode(DataPacket data) override;
};
