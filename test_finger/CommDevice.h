#pragma once
#include"stdafx.h"

class CommDevice{
private:
    CSerial serial;
    int id;
public:
    CommDevice();
    int getConnectId();
    bool connect(int id,int baud);
    bool disconnect();
    void request(std::vector<DataPacket>&const dataPacket);
};
