#pragma once
#include"stdafx.h"

class Comm{
public:
    Comm();
    DataPacket getType();
    int getConnectId();
    bool connect(int id,int baud);
    bool disconnect();
    void request(int CmdCode,uint8_t* Data=0,uint16_t Len=0);
private:
    CSerial serial;
    int id;
    DataPacket type;

    HANDLE responseThread;
    HANDLE listenThread;
    bool startListen();
    bool terminateListen();
};

extern Comm comm;
