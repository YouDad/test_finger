#include "stdafx.h"

void MainDialog::OnBnClickedBtnreadreg(){
    if(getText(cmbProtocolType)=="Syno"){
        progress->SetPos(0);
        MyLog::user("不支持的操作");
        return;
    }
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        MyLog::debug("开始读寄存器");
        uint8_t address=getHex(editReadRegAddr);
        comm.request(SII(ReadRegister),DataPacket(&address,1));
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        MainDialogCtrlValidity::Work();
        progress->SetPos(100*++FlowID/Flow.size());
        FlowID=0;
        Flow.clear();
        return false;
    });
    ExecStart();
}

void MainDialog::OnBnClickedBtnwritereg(){
    if(getText(cmbProtocolType)=="Syno"){
        progress->SetPos(0);
        MyLog::user("不支持的操作");
        return;
    }
    MainDialogCtrlValidity::Working();
    progress->SetPos(50);
    MyLog::debug("开始写寄存器");

    uint8_t addrVal[2];
    addrVal[0]=getHex(editWriteRegAddr);
    addrVal[1]=getHex(editWriteRegVal);

    comm.request(SII(WriteRegister),DataPacket(addrVal,2));

    progress->SetPos(100);
    MainDialogCtrlValidity::Work();
}
