#pragma once
#include"stdafx.h"

class Comm{
public:
    Comm();
    int getConnectId();
    bool connect(int id,int baud);
    bool connectUSB();
    bool disconnect();
    void request(int CmdCode,uint8_t* Data=0,uint16_t Len=0);
private:
    CSerial serial;
    int id;

    HANDLE responseThread;
    HANDLE listenThread;
    bool startListen();
    bool terminateListen();
    void sendBytes(uint8_t* data,int len);
};

extern Comm comm;
extern int CommType;
