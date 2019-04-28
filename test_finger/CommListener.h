#pragma once
#include"stdafx.h"

class ICommListener{
public:
    virtual void listen(DataPacket response)=0;
};

class CommBoardcast{
public:
    CommBoardcast();
    void attach(int event,ICommListener*listener);
    void boardcast(int event,DataPacket response);
private:
    std::map<int,std::vector<ICommListener*>>m;
};

extern CommBoardcast boardcastListener;
