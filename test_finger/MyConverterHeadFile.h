#pragma once
#include"stdafx.h"

#pragma pack(1)
// GD32所用的请求数据包格式
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

// GD32的请求转化器
class RequestConverterGD32F30:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};

#pragma pack(1)
// GD32所用的回应数据包格式
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

// GD32的回应转化器
class ResponseConverterGD32F30:public ICommProtocolResponseConverter{
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket& data) override;
    virtual int getCmdCode(DataPacket data) override;
};

#pragma pack(1)
// LOG所用的数据包格式
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

// LOG的请求转化器
class ResponseConverterLOG:public ICommProtocolResponseConverter{
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket& data) override;
    virtual int getCmdCode(DataPacket data) override;
};

#pragma pack(1)
// Syno所用的数据包格式,有大小端转换代码
struct DataPacketSyno{
    char head[2];//0xEF01
    unsigned int address;//缺省0xFFFFFFFF
    unsigned char sign;
    unsigned short len;
    unsigned char data[140];//命令包中:data[0]是cmd
    unsigned short sum;
    //小端转化成大端
    void convert(){
        sum+=2;
        rev(&sum,2);
        memcpy(data+len,&sum,2);
        len+=2;
        rev(&len,2);
        rev(&address,4);
    }
    //大端转化成小端
    void reconvert(){
        rev(&address,4);
        rev(&len,2);
        len-=2;
        memcpy(&sum,data+len-(sign==SynoSign::ACK),2);
        rev(&sum,2);
    }
private:
    void rev(void* field,int size){
        unsigned char tmp[160];
        unsigned char* p;
        p=(unsigned char*)field;
        memcpy(tmp,p,size);
        for(int i=0;i<size;i++){
            p[i]=tmp[size-1-i];
        }
    }
};
#pragma pack(4)

// Syno的请求转化器
class RequestConverterSyno:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};

// Syno的回应转化器
class ResponseConverterSyno:public ICommProtocolResponseConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket) override;
    virtual DataPacket convert(DataPacket & data) override;
    virtual int getCmdCode(DataPacket data) override;
};
