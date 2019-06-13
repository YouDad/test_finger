#pragma once
#include"stdafx.h"

class Comm{
public:
    Comm();
    int getConnectId();
    bool connect(int id,int baud);
    bool connectUSB();
    bool disconnect();
    void request(int cmdCode,DataPacket packet=DataPacket());
private:
    CSerial serial;
    CUsbPort usb;
    int id;

    MyThread* responseThread;
    MyThread* listenThread;
    void sendBytes(uint8_t* data,int len);
    bool readBytes(uint8_t* buffer,int blen,ULONG& cnt);
    bool isOpen();
};

extern Comm comm;
extern int CommType;
