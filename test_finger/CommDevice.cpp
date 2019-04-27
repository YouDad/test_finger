#pragma once
#include"stdafx.h"

CommDevice::CommDevice(){}

int CommDevice::getConnectId(){
    return id;
}

bool CommDevice::connect(int id,int baud){
    CString way;
    way.Format(_T("\\\\.\\COM%d"),id);//得到端口地址
    LONG retCode=ERROR_SUCCESS;

    if(retCode==ERROR_SUCCESS){
        retCode=serial.Open(way,2048,2048,true);
    }
    //其余三参数均采用默认参数
    if(retCode==ERROR_SUCCESS){
        retCode=serial.Setup((CSerial::EBaudrate)baud);
    }
    if(retCode==ERROR_SUCCESS){
        retCode=serial.SetMask(CSerial::EEventRecv);
    }
    if(retCode==ERROR_SUCCESS){
        serial.Purge();
    }
    MyLog.print(Log::LOGU,"连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
    if(retCode==ERROR_SUCCESS){
        this->id=id;
    }
    return retCode==ERROR_SUCCESS;
}

bool CommDevice::disconnect(){
    int ret=serial.Close();
    MyLog.print(Log::LOGU,"断开连接成功");
    if(ret==ERROR_SUCCESS){
        id=0;
        return true;
    } else{
        ASF_WARNING(5);
        return false;
    }
}

void CommDevice::request(std::vector<DataPacket>& const dataPacket){
    DWORD sendCnt;
    LONG result;
    std::vector<DataPacket>::iterator it;
    for(it=dataPacket.begin();it!=dataPacket.end();it++){
        result=serial.Write(it->data,it->len,&sendCnt,NULL,10*1000);
        it->Destruction();
    }
}
