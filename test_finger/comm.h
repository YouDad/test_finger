#pragma once
#include"stdafx.h"

class Comm{
public:
    Comm();
    uint16_t getType();
    int getConnectId();
    bool connect(int id,int baud);
    bool disconnect();
    void request(int CmdCode,uint8_t* Data=0,uint16_t Len=0);
    void attach(ICommProtocolRequestConverter* converter);
    void attach(ICommProtocolResponseConverter* converter);
private:
    CSerial serial;
    int id;
    uint16_t type;
    std::vector<ICommProtocolRequestConverter*> requestVector;
    std::vector<ICommProtocolResponseConverter*> responseVector;

    HANDLE responseThread;
    HANDLE listenThread;
    bool startListen();
    bool terminateListen();
};

extern Comm comm;
