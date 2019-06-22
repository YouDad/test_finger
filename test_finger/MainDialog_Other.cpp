#include "stdafx.h"

// 日志保存按钮点击事件
void MainDialog::OnBnClickedBtnSaveLog(){
    MyFile::LogSaveAs(
        [&](FILE* fp){
            fprintf_s(fp,"%s",(const char*)getText(editLog));
        }
    );
}

// 开发日志按钮点击事件
void MainDialog::OnBnClickedBtnDevLog(){
    MyLog::DevelopLog();
}

// 狂点调试框的事件
void MainDialog::OnCbnCloseupCmbLogLevel(){
    static clock_t firstTime=0,secondTime=0;
    if(firstTime==0){
        firstTime=clock();
    } else{
        if(secondTime==0){
            secondTime=clock();
        } else{
            clock_t now=clock();
            if(now-firstTime>2000){
                firstTime=secondTime;
                secondTime=now;
            } else{
                // 开启高级调试模式
                conf["AdvDbg"]=Stringify(true);
                btnAdvDbg->ShowWindow(SW_SHOW);
            }
        }
    }
}

// 设备信息按钮点击事件
void MainDialog::OnBnClickedBtnDeviceInfo(){
    // 定义流程
    flow.clear();
    // 流程 0:发送<获得设备信息>命令
    flow.add(0,[](int& result){
        MainDialogCtrlValidity::Working();
        comm.request(SII(DeviceInfo));
        setProgress(100*flow.percent());
        flow.next();
        return false;
    });
    // 流程 1:获得设备信息结束,善后工作
    flow.add(1,[](int& result){
        setProgress(100*flow.percent());
        MyLog::user("获取设备信息结束");
        flow.clear();
        MainDialogCtrlValidity::Work();
        return false;
    });
    // 开始执行流程
    flow.start();
}

// 撤销操作点击事件
void MainDialog::OnBnClickedBtnCancel(){
    flow.terminate();
    setProgress(0);
    MyLog::user("取消了操作");
    MainDialogCtrlValidity::Work();
}

// 清理日志框点击事件
void MainDialog::OnBnClickedBtnClearLog(){
    setText(editLog,"");
}

// 设置按钮点击事件
void MainDialog::OnBnClickedBtnSetting(){
    static SettingDialog* dialog;
    if(dialog){
        delete dialog;
    }
    dialog=new SettingDialog();
    dialog->Create(IDD_SETTING_DIALOG,this);
    dialog->ShowWindow(SW_SHOW);
}
