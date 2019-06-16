#include "stdafx.h"

// 协议变动事件
void MainDialog::OnCbnSelChangeCmbProtocolType(){
    // 存入配置
    conf["ProtocolType"]=MyString::Format("%d",cmbProtocolType->GetCurSel());
    // 重新设置控件可用性
    MainDialogCtrlValidity::Work();
    // 设置串口阻塞态
    if(getText(cmbProtocolType)==SYNO){
        comm.setBlock(true);
    } else{
        comm.setBlock(false);
    }
}

// 高级调试模式点击事件
void MainDialog::OnBnClickedBtnAdvDbg(){
    if(advancedDebugDialog){
        delete advancedDebugDialog;
        advancedDebugDialog=0;
    } else{
        advancedDebugDialog=new AdvancedDebugDialog();
        advancedDebugDialog->Create(IDD_ADVANCED_DEBUG_DIALOG,this);
        advancedDebugDialog->ShowWindow(SW_SHOW);
    }
}
