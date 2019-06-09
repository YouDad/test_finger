#include "stdafx.h"

//连接下位机按钮的点击事件
void MainDialog::OnBnClickedBtnconnect(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnConnect)=="连接下位机"){
        MainDialogCtrlValidity::Connecting();

        bool ret;
        if(getText(cmbWay)=="USB"){
            ret=comm.connectUSB();
        } else{
            int com,baud=getInt(cmbBaud);//读取波特率
            sscanf(getText(cmbWay),"COM%d",&com);//读取通信方式
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
