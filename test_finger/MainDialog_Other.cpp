#include "stdafx.h"

//日志保存按钮点击事件
void MainDialog::OnBnClickedBtnsavelog(){
    //获取文件路径名
    LPCTSTR filter=_T("文本文件(*.txt)|*.txt||");
    CFileDialog dlgFileOpen(0,0,0,0,filter,0);
    if(dlgFileOpen.DoModal()==IDOK){
        MyString path=dlgFileOpen.GetPathName();
        //Add .txt
        if(path.find(".txt",path.length()-4)==-1){
            path=path+".txt";
        }
        //write into file
        FILE* fp=fopen(path,"w");
        fprintf_s(fp,"%s",(const char*)getText(editLog));
        fclose(fp);
    }
}

void MainDialog::OnBnClickedBtndevlog(){
    MyLog::DevelopLog();
}

void MainDialog::OnCbnCloseupCmbloglevel(){
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
                conf["AdvDbg"]="true";
                btnAdvDbg->ShowWindow(SW_SHOW);
            }
        }
    }
}

void MainDialog::OnBnClickedBtndeviceinfo(){
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        comm.request(SII(DeviceInfo));
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        progress->SetPos(100*++FlowID/Flow.size());
        MainDialogCtrlValidity::Work();
        MyLog::user("获取设备信息结束");
        FlowID=0;
        Flow.clear();
        return false;
    });
    ExecStart();
}

void MainDialog::OnBnClickedBtncancel(){
    progress->SetPos(0);
    ExecEnd();
    MyLog::user("取消了操作");
    MainDialogCtrlValidity::Work();
}

void MainDialog::OnBnClickedBtnclearlog(){
    MyLog::ClearLog();
}

void MainDialog::OnBnClickedBtnsetting(){
    static SettingDialog* dialog;
    if(dialog){
        delete dialog;
    }
    dialog=new SettingDialog();
    dialog->Create(IDD_SETTING_DIALOG,this);
    dialog->ShowWindow(SW_SHOW);
}
