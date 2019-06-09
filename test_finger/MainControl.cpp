#pragma once
#include "stdafx.h"

CEdit* editLog;
CEdit* editNow;
CEdit* editAddress;
CEdit* editPassword;
CEdit* editReadRegAddr;
CEdit* editReadRegVal;
CEdit* editWriteRegAddr;
CEdit* editWriteRegVal;
CEdit* editFingerId;
CComboBox* cmbWay;
CComboBox* cmbBaud;
CComboBox* cmbProtocolType;
CComboBox* cmbLogLevel;
CButton* btnConnect;
CButton* btnAdvDbg;
CButton* btnRawImage;
CButton* btnTestImage;
CButton* btnContinueImage;
CButton* btnContinueBGImg;
CButton* btnSaveLog;
CButton* btnReadReg;
CButton* btnWriteReg;
CButton* btnEnroll;
CButton* btnMatch;
CButton* btnDeviceInfo;
CButton* btnViewEnrollIds;
CButton* btnCancel;
CButton* btnClearLog;
CStatic* image;
CProgressCtrl* progress;

HWND hwnd;

AdvancedDebugDialog* advancedDebugDialog;

void initMainControl(MainDialog* Dlg){
    editLog=(CEdit*)Dlg->GetDlgItem(IDC_EDITLog);
    editNow=(CEdit*)Dlg->GetDlgItem(IDC_EDITNow);
    editAddress=(CEdit*)Dlg->GetDlgItem(IDC_EDITAddress);
    editPassword=(CEdit*)Dlg->GetDlgItem(IDC_EDITPassword);
    editReadRegAddr=(CEdit*)Dlg->GetDlgItem(IDC_EDITreadRegAddr);
    editReadRegVal=(CEdit*)Dlg->GetDlgItem(IDC_EDITreadRegVal);
    editWriteRegAddr=(CEdit*)Dlg->GetDlgItem(IDC_EDITwriteRegAddr);
    editWriteRegVal=(CEdit*)Dlg->GetDlgItem(IDC_EDITwriteRegVal);
    editFingerId=(CEdit*)Dlg->GetDlgItem(IDC_EDITFingerId);
    cmbWay=(CComboBox*)Dlg->GetDlgItem(IDC_CMBWay);
    cmbBaud=(CComboBox*)Dlg->GetDlgItem(IDC_CMBBaud);
    cmbProtocolType=(CComboBox*)Dlg->GetDlgItem(IDC_CMBProtocolType);
    cmbLogLevel=(CComboBox*)Dlg->GetDlgItem(IDC_CMBLogLevel);
    btnConnect=(CButton*)Dlg->GetDlgItem(IDC_BTNConnect);
    btnAdvDbg=(CButton*)Dlg->GetDlgItem(IDC_BTNAdvDbg);
    btnRawImage=(CButton*)Dlg->GetDlgItem(IDC_BTNRawImage);
    btnTestImage=(CButton*)Dlg->GetDlgItem(IDC_BTNTestImage);
    btnContinueImage=(CButton*)Dlg->GetDlgItem(IDC_BTNContinueImage);
    btnContinueBGImg=(CButton*)Dlg->GetDlgItem(IDC_BTNContinueBGImg);
    btnSaveLog=(CButton*)Dlg->GetDlgItem(IDC_BTNSaveLog);
    btnReadReg=(CButton*)Dlg->GetDlgItem(IDC_BTNreadReg);
    btnWriteReg=(CButton*)Dlg->GetDlgItem(IDC_BTNwriteReg);
    btnEnroll=(CButton*)Dlg->GetDlgItem(IDC_BTNEnroll);
    btnMatch=(CButton*)Dlg->GetDlgItem(IDC_BTNMatch);
    btnDeviceInfo=(CButton*)Dlg->GetDlgItem(IDC_BTNDeviceInfo);
    btnViewEnrollIds=(CButton*)Dlg->GetDlgItem(IDC_BTNViewEnrollIds);
    btnCancel=(CButton*)Dlg->GetDlgItem(IDC_BTNCancel);
    btnClearLog=(CButton*)Dlg->GetDlgItem(IDC_BTNClearLog);
    image=(CStatic*)Dlg->GetDlgItem(IDC_IMAGE);
    progress=(CProgressCtrl*)Dlg->GetDlgItem(IDC_PROGRESS);

    editLog->SetLimitText(-1);
    setText(editNow,MyString::Format("自动更新是否开启:%s\r\n",conf["AutoCheck"].c_str()));
    hwnd=Dlg->m_hWnd;

    if(conf["AdvDbg"]=="true"){
        btnAdvDbg->ShowWindow(SW_SHOW);
    }
    advancedDebugDialog=0;

    //常用波特率
    MyString baud[]={"9600","19200","57600","115200","230400","460800","921600"};
    for(int i=0;i<7;i++){
        cmbBaud->InsertString(i,baud[i]);
    }
    cmbBaud->SetCurSel(MyString::ParseInt(conf["Baud"]));

    //芯片类型
    MyString chipType[]={"GD32F30","ASFComm","Syno","SuUSB"};
    for(int i=0;i<4;i++){
        cmbProtocolType->InsertString(i,chipType[i]);
    }
    cmbProtocolType->SetCurSel(MyString::ParseInt(conf["ProtocolType"]));

    //日志信息等级
    MyString logLevel[]={"用户","错误","警告","调试","临时"};
    for(int i=0;i<5;i++){
        cmbLogLevel->InsertString(i,logLevel[i]);
    }
    cmbLogLevel->SetCurSel(3);

    //进度条设置
    progress->SetRange(0,100);
    progress->SetPos(0);

    //设置标题
    int BigVersion=Version/100;
    int SmlVersion=Version%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    setText(Dlg,getText(Dlg)+MyString::Format(" Ver%d.%d",BigVersion,SmlVersion));

    //各控件访问权限
    MainDialogCtrlValidity::InitCtrl();
    MainDialogCtrlValidity::Init();

    //刷新通信方式
    updateCommunityWay();

    //自动检查更新
    if(conf["AutoCheck"]=="true"){
        if(isConnectedNet()){
            if(NetGetVersion()>Version){
                MyLog::user("有可更新版本,在设置中更新.");
            }
        }
    }
}

void sendMainDialogMessage(int Message){
    SendMessage(hwnd,Message,Message,0);
}

std::vector<int>* idle=new std::vector<int>();
std::vector<int>* lastIdle=NULL;

void updateCommunityWay(){
    if(lastIdle){
        delete lastIdle;
    }
    lastIdle=idle;
    idle=new std::vector<int>();

    //清除串口数组内容
    idle->clear();
    //因为至多有255个串口，所以依次检查各串口是否存在
    //如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，
    //都认为串口存在，只不过后者表明串口已经被占用,否则串口不存在
    //检查255个太费时间,且用前30个的几率较大
    for(int i=1; i<30; i++){
        //生成原始设备名
        CString sPort;
        sPort.Format(_T("\\\\.\\COM%d"),i);

        HANDLE hPort=::CreateFile(sPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
        if(hPort!=INVALID_HANDLE_VALUE){
            idle->push_back(i);
            CloseHandle(hPort);
        }
    }

    //UI:组合框 更新
    cmbWay->ResetContent();
    if(idle->size()==0){
        MyLog::user("未发现空闲的串口");
        return;
    } else{
        MyLog::user("发现%d个串口",idle->size());
        for(int i=0;i<idle->size();i++){
            CString name;
            name.Format(_T("COM%d"),(*idle)[i]);
            cmbWay->InsertString(i,name);
        }
    }
    //对USB的支持
    cmbWay->InsertString(0,_T("USB"));
}


void autoConnect(){
    int id=comm.getConnectId();
    //自动连接只在未连接状态生效
    if(id==0){
        if(idle->size()==lastIdle->size()+1){ //应该会增加一个空闲设备
            std::vector<int> diff;
            std::set_difference(
                idle->begin(),idle->end(),
                lastIdle->begin(),lastIdle->end(),
                std::inserter(diff,diff.begin())
            );//求idle-lastIdle
            if(diff.size()==1){ //差集应该只有一个元素
                int needConnectId=diff[0];
                for(int i=0;i<idle->size();i++){
                    if((*idle)[i]==needConnectId){
                        cmbWay->SetCurSel(i);
                        bool ret=comm.connect(needConnectId,getInt(cmbBaud));
                        if(ret){
                            setText(btnConnect,"断开连接");
                            MainDialogCtrlValidity::AfterConnect();
                        } else{
                            MainDialogCtrlValidity::BeforeConnect();
                        }
                    }
                }
            } else{
                MyLog::error("发现Bug,当前串口处于未连接状态,经过一次串口枚举,发现idle和lastIdle的差集元素个数不是一个,详细信息:");
                {
                    MyString error="diff:";
                    for(std::vector<int>::iterator it=diff.begin();it!=diff.end();it++){
                        char number[20];
                        sprintf(number," %d",*it);
                        error+=number;
                    }
                    MyLog::error(error);
                }
            }
        } else{
            //其他异常情况
            MyLog::error("发现Bug,当前串口处于未连接状态,经过一次串口枚举,发现idle并不比lastIdle多,详细信息:");
            {
                MyString error="idle:";
                for(std::vector<int>::iterator it=idle->begin();it!=idle->end();it++){
                    char number[20];
                    sprintf(number," %d",*it);
                    error+=number;
                }
                MyLog::error(error);
            }
            {
                MyString error="lastIdle:";
                for(std::vector<int>::iterator it=lastIdle->begin();it!=lastIdle->end();it++){
                    char number[20];
                    sprintf(number," %d",*it);
                    error+=number;
                }
                MyLog::error(error);
            }
        }
    }
}

void autoDisconnect(){
    int id=comm.getConnectId();
    //自动断开仅当连接状态下
    if(id!=0){
        bool needDisconnect=true;
        for(int i=0;i<idle->size();i++){
            if((*idle)[i]==id){
                needDisconnect=false;
            }
        }
        if(needDisconnect){
            comm.disconnect();
            MainDialogCtrlValidity::BeforeConnect();
            btnConnect->SetWindowText(_T("连接下位机"));
        }
    }
}
