#pragma once
#include"stdafx.h"

class CmdCodeLOG_AdvDbg_AdjImg_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_Info_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_MeasureTimeStart_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_MeasureTimeEnd_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};
