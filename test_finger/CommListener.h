#pragma once
#include"stdafx.h"

interface ICommListener{
    void listen(DefaultResponsePacket response);
};

class CommBoardcast{
public:
    void attach(int event,ICommListener listener);
    void boardcast(int event,DefaultResponsePacket response);
private:
    std::map<int,std::vector<ICommListener>>m;
};

class GET_RAW_IMAGE_Listener:public ICommListener{
    void listen(DefaultResponsePacket response);
};