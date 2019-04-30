#include "stdafx.h"
#include "CmdCodeGD32F30.h"

void READ_REGISTER_Listener::listen(DataPacket response){
    if(response.size()==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        setText(editReadRegVal,MyString::Format("%X",response.getPointer()[0]));
        MyLog.print(Log::LOGU,"接收数据成功");
    }
    sendMessage(WM_READ_REGISTER);
    updateControlDisable(actReadedReg);
}
