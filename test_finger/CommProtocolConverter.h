#pragma once
#include"stdafx.h"

class DataPacket{
public:
    BYTE* data;
    int   len;
    int read;
    DataPacket(const void* d,int l);
    DataPacket();
    void Destruction();
    bool isValid();
};

class ICommProtocolConverter{
public:
    virtual bool checkProtocol(uint16_t head)=0;
};

class ICommProtocolRequestConverter:public ICommProtocolConverter{
public:
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t* Data,uint16_t Len)=0;
};

class ICommProtocolResponseConverter:public ICommProtocolConverter{
public:
    virtual DataPacket convert(DataPacket& data)=0;
    virtual int getCmdCode(DataPacket data)=0;
};

enum ProtocolSign{
    RequestNotEnd,
    RequestEnd,
    ResponseNotEnd,
    ResponseEnd
};