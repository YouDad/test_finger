#pragma once
#include"stdafx.h"

class GET_RAW_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class GET_TEST_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class READ_REGISTER_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
