#pragma once
#include"stdafx.h"

class ICommProtocolConverter{
public:
    virtual bool checkProtocol(DataPacket dataPacket)=0;
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

class ConverterBoardcast{
public:
    ConverterBoardcast();
    ICommProtocolRequestConverter* RequestConvert();
    DataPacket ResponseConvert(DataPacket& data);
    int ResponseGetCmdCode(DataPacket data);
private:
    void attach(ICommProtocolRequestConverter* converter);
    void attach(ICommProtocolResponseConverter* converter);
    std::vector<ICommProtocolRequestConverter*> requestVector;
    std::vector<ICommProtocolResponseConverter*> responseVector;
};

extern ConverterBoardcast converterBoardcast;

enum ProtocolSign{
    Unspecified,
    DataNotEnd,
    DataEnd,
};

enum SynoSign{
    Command=1,
    NotEndData=2,
    ACK=7,
    EndData=8,
};
