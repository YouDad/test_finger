#include "stdafx.h"

void MainDialog::OnBnClickedBtn(){
    setText(editFingerId,getText(GetFocus()));
}

void MainDialog::OnBnClickedBtnenroll(){
    if(getText(editFingerId)==""){
        MyLog::user("编号不能为空");
        return;
    }
    static uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
    static uint8_t BufferID;
    BufferID=1;
    static const int EnrollCount=3;
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        comm.request(SII(GetEnrollImage));//获取图像
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        if(result==0x00){//取到图像
            MyLog::debug("取到指纹图像(%d/%d)",BufferID,EnrollCount);
            comm.request(SII(UpImage));//上传图像
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{//没取到图像
            FlowID--;
            return true;
        }
    });
    Flow.push_back(FlowFunction(2)(int& result){
        if(result==0x00){//上传图像成功
            comm.request(SII(GenChar),&BufferID,1);//生成特征
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{//上传图像失败
            MyLog::debug("上传图像失败");
            FlowID=11;
            MyLog::user("注册失败");
            return true;
        }
    });
    Flow.push_back(FlowFunction(3)(int& result){
        if(result==0x00){//生成特征成功
            FlowID++;
            MyLog::debug("生成特征成功(%d/%d)",BufferID,EnrollCount);

            uint8_t x[]={BufferID,0,0,0,0x87};
            comm.request(SII(Search),x,sizeof x);//搜索指纹
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{//生成特征失败
            FlowID=0;
            return true;
        }
    });
    Flow.push_back(FlowFunction(4)(int& result){
        if(result==0x09){//没搜到指纹
            MyLog::debug("等待手指移开");
            comm.request(SII(GetRawImage));//等待把手指移开
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{//搜到指纹
            MyLog::debug("已有指纹");
            FlowID=11;
            MyLog::user("注册失败");
            return true;
        }
    });
    Flow.push_back(FlowFunction(5)(int& result){
        if(result==0x02){//没指纹
            FlowID++;
        } else{//有指纹
            result=0x09;
            FlowID--;
        }
        return true;
    });
    Flow.push_back(FlowFunction(6)(int& result){
        if(BufferID>=EnrollCount){//达到录入次数
            FlowID++;
        } else{//没达到录入次数
            BufferID++;
            FlowID=0;
        }
        return true;
    });
    Flow.push_back(FlowFunction(7)(int& result){
        comm.request(SII(RegModel));//合并特征
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(8)(int& result){
        if(result==0x00){//合并特征成功
            FlowID++;
            MyLog::debug("合并特征成功");
        } else{//合并失败
            FlowID=11;
            MyLog::user("注册失败");
        }
        return true;
    });
    Flow.push_back(FlowFunction(9)(int& result){
        uint8_t data[]={1,0,FingerID};
        comm.request(SII(StoreChar),data,sizeof data);//存储模板
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(10)(int& result){
        if(result==0x00){//存储模板成功
            FlowID++;
            MyLog::debug("存储模板成功");
        } else{//存储失败
            FlowID=11;
            MyLog::user("注册失败");
        }
        return true;
    });
    Flow.push_back(FlowFunction(11)(int& result){//注册结束
        progress->SetPos(100*++FlowID/Flow.size());
        FlowID=0;
        BufferID=1;
        MyLog::user("注册结束");
        Flow.clear();
        MainDialogCtrlValidity::Work();
        return false;
    });
    ExecStart();
}

void MainDialog::OnBnClickedBtnmatch(){
    if(getText(editFingerId)==""){
        MyLog::user("编号不能为空");
        return;
    }
    static uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        uint8_t data[]={02,00,FingerID};
        comm.request(SII(LoadChar),data,sizeof data);
        FlowID++;
        MyLog::user("读出指纹模板中...");
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        if(result==0x00){
            comm.request(SII(GetRawImage));
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{
            FlowID=5;
            return true;
        }
    });
    Flow.push_back(FlowFunction(2)(int& result){
        if(result==0x00){
            MyLog::user("取指纹图成功");
            uint8_t data[]={02};
            comm.request(SII(GenChar),data,sizeof data);
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{
            result=0x00;
            FlowID--;
            return true;
        }
    });
    Flow.push_back(FlowFunction(3)(int& result){
        if(result==0x00){
            MyLog::user("指纹生成特征成功");
            comm.request(SII(Match));
            progress->SetPos(100*++FlowID/Flow.size());
            return false;
        } else{
            MyLog::user("指纹生成特征失败");
            result=0x00;
            FlowID--;//回退两步
            FlowID--;//回退两步
            return true;
        }
    });
    Flow.push_back(FlowFunction(4)(int& result){
        if(result==0x00){
            MyLog::user("指纹匹配成功");
            FlowID++;
            return true;
        } else{
            MyLog::user("指纹不匹配");
            FlowID++;
            return true;
        }
    });
    Flow.push_back(FlowFunction(5)(int& result){
        progress->SetPos(100*++FlowID/Flow.size());
        MainDialogCtrlValidity::Work();
        MyLog::user("比对结束");
        FlowID=0;
        Flow.clear();
        return false;
    });
    ExecStart();
}

void MainDialog::OnBnClickedBtnviewenrollids(){
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        uint8_t data[]={0};
        comm.request(SII(ReadIndexTable),data,sizeof data);
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        progress->SetPos(100*++FlowID/Flow.size());
        MainDialogCtrlValidity::Work();
        FlowID=0;
        Flow.clear();
        return false;
    });
    ExecStart();
}

void MainDialog::OnBnClickedBtndeletetemplate(){
    if(getText(editFingerId)==""){
        MyLog::user("编号不能为空");
        return;
    }
    static uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        MainDialogCtrlValidity::Working();
        uint8_t data[]={0,FingerID,0,1};
        comm.request(SII(DeleteChar),data,sizeof data);
        progress->SetPos(100*++FlowID/Flow.size());
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        if(result==0x00){
            MyLog::user("删除指纹%d成功",FingerID);
        }
        progress->SetPos(100*++FlowID/Flow.size());
        MainDialogCtrlValidity::Work();
        FlowID=0;
        Flow.clear();
        return false;
    });
    ExecStart();
}
