#include "stdafx.h"

// 打开图片文件夹点击事件
void MainDialog::OnBnClickedBtnOpenImage(){
    if(access("collectedImage",0)){
        MyLog::user("图片文件夹不存在,请先采一张图片");
    } else{
        ShellExecuteA(NULL,"explore","collectedImage",NULL,NULL,SW_NORMAL);
    }
}

// 打开背景文件夹点击事件
void MainDialog::OnBnClickedBtnOpenBackgroundImage(){
    if(access("collectedBGI",0)){
        MyLog::user("背景文件夹不存在,请先采一张背景");
    } else{
        ShellExecuteA(NULL,"explore","collectedBGI",NULL,NULL,SW_NORMAL);
    }
}

//原始图像的点击事件
void MainDialog::OnBnClickedBtnRawImage(){
    if(getProtocol()==SYNO){
        MyLog::user("开始获取图像");
        // 定义流程
        flow.clear();
        // 流程 0:发送<获得图像>命令
        flow.add(0,[](int& result){
            MainDialogCtrlValidity::Working();
            comm.request(SII(GetRawImage));
            setProgress(100*flow.percent());
            flow.next();
            return false;
        });
        // 流程 1:如果取到图,发送<上传图像>命令
        // 流程 1:如果没取到图,回到上一个流程
        flow.add(1,[](int& result){
            if(result==0x00){
                MainDialogCtrlValidity::Working();
                comm.request(SII(UpImage));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            } else{
                flow.prev();
                return true;
            }
        });
        // 流程 2:获取图像结束,善后工作
        flow.add(2,[](int& result){
            setProgress(100*flow.percent());
            MyLog::user("获取图像完毕");
            flow.clear();
            MainDialogCtrlValidity::Work();
            return false;
        });
        // 开始执行流程
        flow.start();
    }
    if(getProtocol()==GD32){
        MainDialogCtrlValidity::Working();
        // 发送<获取原始图像>命令
        comm.request(SII(GetRawImage));
        setProgress(30);
        MyLog::user("请放手指");
    }
}

void MainDialog::OnBnClickedBtnBackgroundImage(){
    if(getProtocol()==GD32){
        MainDialogCtrlValidity::Working();
        setProgress(30);
        MyLog::debug("开始采集背景");
        comm.request(SII(GetTestImage));
        setProgress(60);
    }
}

void MainDialog::OnBnClickedBtnContinueBackgroundImage(){
    if(getProtocol()==GD32){
        // 根据按钮上的文字判断当前连接状态
        if(getText(btnContinueBGImg)=="连续获取背景"){
            MyLog::user("开始连续获取背景");
            MainDialogCtrlValidity::Working(MainDialogCtrlValidity::vec{btnContinueBGImg});
            setText(btnContinueBGImg,"停止获取背景");
            sendMainDialogMessage(WM_GET_CON_BKI);
        } else{
            MyLog::user("停止连续获取背景");
            MainDialogCtrlValidity::Work();
            setText(btnContinueBGImg,"连续获取背景");
            sendMainDialogMessage(WM_STP_GET_BKI);
        }
    }
}

//连续获取图像的点击事件
void MainDialog::OnBnClickedBtnContinueImage(){
    if(getProtocol()==SYNO){
        MyLog::user("开始连续获取图像");
        // 定义流程
        flow.clear();
        // 流程 0:发送<获得图像>命令
        flow.add(0,[](int& result){
            MainDialogCtrlValidity::Working();
            comm.request(SII(GetRawImage));
            setProgress(100*flow.percent());
            flow.next();
            return false;
        });
        // 流程 1:如果取图成功,发送<上传图像>命令
        // 流程 1:如果取图失败,回到上一个流程
        flow.add(1,[](int& result){
            if(result==0x00){
                MainDialogCtrlValidity::Working();
                comm.request(SII(UpImage));
                setProgress(100*flow.percent());
                flow.next();
                return false;
            } else{
                flow.prev();
                return true;
            }
        });
        // 流程 2:回到流程 0
        flow.add(2,[](int& result){
            setProgress(100*flow.percent());
            flow.jump(0);
            return true;
        });
        // 流程 3:连续取图结束,善后工作
        flow.add(3,[](int& result){
            setProgress(100*flow.percent());
            MyLog::user("停止连续获取图像");
            flow.clear();
            MainDialogCtrlValidity::Work();
            return false;
        });
        flow.start();
    }
    if(getProtocol()==GD32){
        //根据按钮上的文字判断当前连接状态
        if(getText(btnContinueImage)=="连续获取图像"){
            MyLog::user("开始连续获取图像");
            MainDialogCtrlValidity::Working(MainDialogCtrlValidity::vec{btnContinueImage});
            setText(btnContinueImage,"停止获取图像");
            sendMainDialogMessage(WM_GET_CON_IMAGE);
        } else{
            MyLog::user("停止连续获取图像");
            MainDialogCtrlValidity::Work();
            setText(btnContinueImage,"连续获取图像");
            sendMainDialogMessage(WM_STP_GET_IMAGE);
        }
    }
}
