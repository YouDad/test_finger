#include "stdafx.h"
#include "CmdCodeLOG_MeasureTime.h"

clock_t clockTime;

void CmdCodeLOG_MeasureTimeStart_Listener::listen(DataPacket response){
    clockTime=clock();
}

void CmdCodeLOG_MeasureTimeEnd_Listener::listen(DataPacket response){
    MyLog.debug("MeasureTime:%d",clock()-clockTime);
}
