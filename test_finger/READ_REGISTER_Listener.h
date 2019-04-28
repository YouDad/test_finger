#pragma once
#include"stdafx.h"

class READ_REGISTER_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
