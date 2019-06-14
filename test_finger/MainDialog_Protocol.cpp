#include "stdafx.h"

void MainDialog::OnCbnSelchangeCmbprotocoltype(){
    conf["ProtocolType"]=MyString::Format("%d",cmbProtocolType->GetCurSel());
    MainDialogCtrlValidity::Work();
    if(getText(cmbProtocolType)==SYNO){
        comm.setBlock(true);
    } else{
        comm.setBlock(false);
    }
}

void MainDialog::OnBnClickedBtnadvdbg(){
    if(advancedDebugDialog){
        delete advancedDebugDialog;
        advancedDebugDialog=0;
    } else{
        advancedDebugDialog=new AdvancedDebugDialog();
        advancedDebugDialog->Create(IDD_ADVANCED_DEBUG_DIALOG,this);
        advancedDebugDialog->ShowWindow(SW_SHOW);
    }
}
