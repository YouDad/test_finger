#include "stdafx.h"

// 读寄存器点击事件
void MainDialog::OnBnClickedBtnReadReg(){
    if(getProtocol()==GD32){
        // 定义流程
        flow.clear();
        // 流程 0:发送<读寄存器>指令
        flow.add(0,
            [](int& result){
                MainDialogCtrlValidity::Working();
                MyLog::debug("开始读寄存器");
                uint8_t address=getHex(editReadRegAddr);
                comm.request(SII(ReadRegister),DataPacket(&address,1));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            }
        );
        // 流程 1:读寄存器结束,善后工作
        flow.add(1,
            [](int& result){
                MainDialogCtrlValidity::Work();
                setProgress(100*flow.percent());
                flow.clear();
                return false;
            }
        );
        // 开始执行流程
        flow.start();
    }
}

// 写寄存器点击事件
void MainDialog::OnBnClickedBtnWriteReg(){
    if(getProtocol()==GD32){
        MainDialogCtrlValidity::Working();
        setProgress(50);
        MyLog::debug("开始写寄存器");

        uint8_t addrVal[2];
        addrVal[0]=getHex(editWriteRegAddr);
        addrVal[1]=getHex(editWriteRegVal);
        // 发送<写寄存器>指令
        comm.request(SII(WriteRegister),DataPacket(addrVal,2));

        setProgress(100);
        MainDialogCtrlValidity::Work();
    }
}
