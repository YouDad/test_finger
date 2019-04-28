#pragma once
#include"stdafx.h"

class GET_RAW_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
