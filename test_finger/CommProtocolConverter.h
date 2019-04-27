#pragma once
#include"stdafx.h"

class DataPacket{
    BYTE* data;
    int   len;
};


class ICommProtocolConverter{
public:
    virtual bool checkProtocol(uint16_t head)=0;
};

template<class T>
class ICommProtocolRequestConverter:public ICommProtocolConverter{
public:
    virtual DataPacket convert(T data)=0;
};

template<class T>
class ICommProtocolResponseConverter:public ICommProtocolConverter{
public:
    virtual T convert(DataPacket data)=0;
};

class RequestConverterDefault:public ICommProtocolRequestConverter<DefaultSendPacket>{

};