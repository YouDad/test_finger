#pragma once
#include"stdafx.h"

class CmdCodeLOG_Info_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
