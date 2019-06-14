#include "stdafx.h"

//连接下位机按钮的点击事件
void MainDialog::OnBnClickedBtnconnect(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnConnect)=="连接下位机"){
        if(getText(cmbWay)==""){
            return;
        }
        MainDialogCtrlValidity::Connecting();

        bool ret;
        if(getText(cmbWay)=="USB"){
            ret=comm.connect();
        } else{
            int baud=getInt(cmbBaud);//读取波特率
            int com=(*idle)[idle->size()-cmbWay->GetCurSel()];
            ret=comm.connect(com,baud);
        }
        if(ret){
            setText(btnConnect,"断开连接");
            MainDialogCtrlValidity::AfterConnect();
        } else{
            MainDialogCtrlValidity::BeforeConnect();
        }
    } else{
        comm.disconnect();
        MainDialogCtrlValidity::BeforeConnect();
        setText(btnConnect,"连接下位机");
    }
}

void MainDialog::OnCbnSelchangeCmbbaud(){
    conf["Baud"]=MyString::Format("%d",cmbBaud->GetCurSel());
}

void MainDialog::OnCbnSelchangeCmbway(){
    if(getText(cmbWay)=="USB"){
        cmbBaud->EnableWindow(FALSE);
    } else{
        cmbBaud->EnableWindow(TRUE);
    }
}
