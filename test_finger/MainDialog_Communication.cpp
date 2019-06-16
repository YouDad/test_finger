#include "stdafx.h"

// 连接下位机按钮的点击事件
void MainDialog::OnBnClickedBtnConnect(){
    // 根据按钮上的文字判断当前连接状态
    if(getText(btnConnect)=="连接下位机"){
        if(getText(cmbWay)==""){
            return;
        }
        MainDialogCtrlValidity::Connecting();

        // 读取串口号和波特率
        int com=getComID();
        int baud=getInt(cmbBaud);
        bool ret=comm.connect(com,baud);
        
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

// 如果波特率变化,保存到配置里
void MainDialog::OnCbnSelChangeCmbBaud(){
    conf["Baud"]=MyString::Format("%d",cmbBaud->GetCurSel());
}

// 如果通信方式选的是USB就把波特率选择禁用
void MainDialog::OnCbnSelChangeCmbWay(){
    if(getText(cmbWay)=="USB"){
        cmbBaud->EnableWindow(FALSE);
    } else{
        cmbBaud->EnableWindow(TRUE);
    }
}
