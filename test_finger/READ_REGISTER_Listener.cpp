#include "stdafx.h"
#include "READ_REGISTER_Listener.h"

void READ_REGISTER_Listener::listen(DataPacket response){
    if(response.len==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        CString tmp;
        tmp.Format(_T("%X"),response.data[0]);
        editReadRegVal->SetWindowText(tmp);
        MyLog.print(Log::LOGU,"接收数据成功");
    }
    updateControlDisable(actReadedReg);
}
