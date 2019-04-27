#pragma once
#include"stdafx.h"

class ICommListener{
public:
    virtual void listen(DefaultResponsePacket response)=0;
};

class CommBoardcast{
public:
    void attach(int event,ICommListener*listener);
    void boardcast(int event,DefaultResponsePacket response);
private:
    std::map<int,std::vector<ICommListener*>>m;
};

class GET_RAW_IMAGE_Listener:public ICommListener{
    void listen(DefaultResponsePacket response);
};