#include "stdafx.h"
#include "READ_REGISTER.h"

void READ_REGISTER_Listener::listen(DataPacket response){
    if(response.size()==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        CString tmp;
        tmp.Format(_T("%X"),response.getPointer()[0]);
        editReadRegVal->SetWindowText(tmp);
        MyLog.print(Log::LOGU,"接收数据成功");
    }
    SendMessage(hwnd,WM_READ_REGISTER,WM_READ_REGISTER,0);
    updateControlDisable(actReadedReg);
}
