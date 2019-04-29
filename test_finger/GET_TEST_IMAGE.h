#pragma once
#include"stdafx.h"

class GET_TEST_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
