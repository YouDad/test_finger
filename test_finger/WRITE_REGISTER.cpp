#include "stdafx.h"
#include "WRITE_REGISTER.h"

void WRITE_REGISTER_Listener::listen(DataPacket response){
    MyLog.print(Log::LOGU,"修改寄存器命令发送成功");
    updateControlDisable(actWritedReg);
}
