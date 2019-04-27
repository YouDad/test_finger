#pragma once
#include"stdafx.h"

class DataPacket{
public:
    BYTE* data;
    int   len;
    DataPacket(const void* d,int l);
    void Destruction();
};

class ICommProtocolConverter{
public:
    virtual bool checkProtocol(uint16_t head)=0;
};

template<class T>
class ICommProtocolRequestConverter:public ICommProtocolConverter{
public:
    virtual std::vector<DataPacket> convert(T CmdCode,uint8_t* Data,uint16_t Len)=0;
};

template<class T>
class ICommProtocolResponseConverter:public ICommProtocolConverter{
public:
    virtual std::vector<T> convert(DataPacket data)=0;
};

enum ProtocolSign{
    RequestNotEnd,
    RequestEnd,
    ResponseNotEnd,
    ResponseEnd
};