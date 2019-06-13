#include "stdafx.h"

void MainDialog::OnBnClickedBtnopenimage(){
    if(access("collectedImage",0)){
        MyLog::user("图片文件夹不存在,请先采一张图片");
    } else{
        ShellExecuteA(NULL,"explore","collectedImage",NULL,NULL,SW_NORMAL);
    }
}

void MainDialog::OnBnClickedBtnopenbackgroundimage(){
    if(access("collectedBGI",0)){
        MyLog::user("背景文件夹不存在,请先采一张背景");
    } else{
        ShellExecuteA(NULL,"explore","collectedBGI",NULL,NULL,SW_NORMAL);
    }
}

//原始图像的点击事件
void MainDialog::OnBnClickedBtnrawimage(){
    if(getText(cmbProtocolType)==SYNO){
        MyLog::user("开始获取图像");
        Flow.clear();
        Flow.push_back(FlowFunction(0)(int& result){
            MainDialogCtrlValidity::Working();
            comm.request(SII(GetRawImage));
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        });
        Flow.push_back(FlowFunction(1)(int& result){
            if(result==0x00){
                MainDialogCtrlValidity::Working();
                comm.request(SII(UpImage));
                progress->SetPos(100*++FlowID/Flow.size());
                return false;
            } else{
                FlowID--;
                return true;
            }
        });
        Flow.push_back(FlowFunction(2)(int& result){
            progress->SetPos(100*++FlowID/Flow.size());
            FlowID=0;
            Flow.clear();
            MyLog::user("获取图像完毕");
            MainDialogCtrlValidity::Work();
            return false;
        });
        ExecStart();
        return;
    }
    MainDialogCtrlValidity::Working();
    comm.request(SII(GetRawImage));
    progress->SetPos(30);
    MyLog::user("请放手指");
}

void MainDialog::OnBnClickedBtnbackgroundimage(){
    if(getText(cmbProtocolType)==SYNO){
        progress->SetPos(0);
        MyLog::user("不支持的操作");
        return;
    }
    MainDialogCtrlValidity::Working();
    progress->SetPos(30);
    MyLog::debug("开始采集背景");
    comm.request(SII(GetTestImage));
    progress->SetPos(60);
}

void MainDialog::OnBnClickedBtncontinuebackgroundimage(){
    if(getText(cmbProtocolType)==SYNO){
        progress->SetPos(0);
        MyLog::user("不支持的操作");
        return;
    }
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueBGImg)=="连续获取背景"){
        MyLog::user("开始连续获取背景");
        MainDialogCtrlValidity::Working(MainDialogCtrlValidity::vec{btnContinueBGImg});
        setText(btnContinueBGImg,"停止获取背景");
        SendMessage(WM_GET_CON_BKI,WM_GET_CON_BKI,0);
    } else{
        MyLog::user("停止连续获取背景");
        MainDialogCtrlValidity::Work();
        setText(btnContinueBGImg,"连续获取背景");
        SendMessage(WM_STP_GET_BKI,WM_STP_GET_BKI,0);
    }
}

//连续获取图像的点击事件
void MainDialog::OnBnClickedBtncontinueimage(){
    if(getText(cmbProtocolType)==SYNO){
        MyLog::user("开始连续获取图像");
        Flow.clear();
        Flow.push_back(FlowFunction(0)(int& result){
            MainDialogCtrlValidity::Working();
            comm.request(SII(GetRawImage));
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        });
        Flow.push_back(FlowFunction(1)(int& result){
            if(result==0x00){
                MainDialogCtrlValidity::Working();
                comm.request(SII(UpImage));
                progress->SetPos(100*++FlowID/Flow.size());
                return false;
            } else{
                FlowID--;
                return true;
            }
        });
        Flow.push_back(FlowFunction(2)(int& result){
            progress->SetPos(100*++FlowID/Flow.size());
            FlowID=0;
            return true;
        });
        Flow.push_back(FlowFunction(3)(int& result){
            progress->SetPos(100*++FlowID/Flow.size());
            FlowID=0;
            Flow.clear();
            MyLog::user("停止连续获取图像");
            MainDialogCtrlValidity::Work();
            return false;
        });
        ExecStart();
        return;
    }
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueImage)=="连续获取图像"){
        MyLog::user("开始连续获取图像");
        MainDialogCtrlValidity::Working(MainDialogCtrlValidity::vec{btnContinueImage});
        setText(btnContinueImage,"停止获取图像");
        SendMessage(WM_GET_CON_IMAGE,WM_GET_CON_IMAGE,0);
    } else{
        MyLog::user("停止连续获取图像");
        MainDialogCtrlValidity::Work();
        setText(btnContinueImage,"连续获取图像");
        SendMessage(WM_STP_GET_IMAGE,WM_STP_GET_IMAGE,0);
    }
}
