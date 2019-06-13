#pragma once
#include"stdafx.h"

class ICommListener{
public:
    ICommListener(const char* protocol=0);
    bool accept();
    virtual void listen(DataPacket response)=0;
private:
    const char* protocol;
};

class ListenerBoardcast{
public:
    ListenerBoardcast();
    void attach(int event,ICommListener*listener);
    void boardcast(int event,DataPacket response);
private:
    std::map<int,std::vector<ICommListener*>>m;
};

extern ListenerBoardcast listenerBoardcast;
