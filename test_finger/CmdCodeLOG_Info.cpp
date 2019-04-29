#include "stdafx.h"
#include "CmdCodeLOG_INFO.h"

void CmdCodeLOG_Info_Listener::listen(DataPacket response){
    MyLog.print(Log::LOGD,(char*)response.getPointer());
}
