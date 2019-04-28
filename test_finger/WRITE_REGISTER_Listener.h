#pragma once
#include"stdafx.h"

class WRITE_REGISTER_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
