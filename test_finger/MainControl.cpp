#pragma once
#include "stdafx.h"

// 控件指针群
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
CButton* btnDeleteTemplate;
CStatic* image;
CProgressCtrl* progress;

HWND hwnd;

AdvancedDebugDialog* advancedDebugDialog;

// 初始化MainDialog的控件指针,以及初始化一些控件设置
void initMainControl(MainDialog* Dlg){
    progress=(CProgressCtrl*)Dlg->GetDlgItem(IDC_PROGRESS);

    //进度条设置
    progress->SetRange(0,100);
    setProgress(10);

    // 控件指针群初始化
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
    btnDeleteTemplate=(CButton*)Dlg->GetDlgItem(IDC_BTNDeleteTemplate);
    image=(CStatic*)Dlg->GetDlgItem(IDC_IMAGE);
    hwnd=Dlg->m_hWnd;

    (new MyThread(
        [&](){
            MyString last="";
            while(1){
                MyString log=MyLog::MsgQueue.pop();
                MyString WithTime=log.Format<const char*>(
                    MyString::Time("%Y-%m-%d %H:%M:%S"));
                if(last!=log){
                    int len=editLog->GetWindowTextLength();
                    MyString old_content=getText(editNow);
                    editLog->SetSel(len,len,0);
                    editLog->ReplaceSel(old_content+"\r");
                    MyLog::appendLog(old_content+"\n");
                }
                if(getText(editNow)!=WithTime){
                    setText(editNow,WithTime);
                }
            }
        }
    ,true))->start();

    setProgress(20);

    //各控件访问权限
    MainDialogCtrlValidity::InitCtrl();

    setProgress(30);

    // 设置日志框
    editLog->SetLimitText(-1);
    setText(editNow,MyString::Format("自动更新是否开启:%s\r\n",conf["AutoCheck"].c_str()));

    // 设置高级调试按钮可见性
    if(conf["AdvDbg"]==Stringify(true)){
        btnAdvDbg->ShowWindow(SW_SHOW);
    }
    advancedDebugDialog=0;

    setProgress(40);

    //常用波特率
    MyString baud[]={"9600","19200","57600","115200","230400","460800","921600"};
    for(int i=0;i<sizeof(baud)/sizeof(baud[0]);i++){
        cmbBaud->InsertString(i,baud[i]);
    }
    cmbBaud->SetCurSel(MyString::ParseInt(conf["Baud"]));

    setProgress(50);

    //芯片类型
    MyString chipType[]={GD32,SYNO};
    for(int i=0;i<sizeof(chipType)/sizeof(chipType[0]);i++){
        cmbProtocolType->InsertString(i,chipType[i]);
    }
    //max是为了升级之后,兼容以前大于等于2的情况
    cmbProtocolType->SetCurSel(max(1,MyString::ParseInt(conf["ProtocolType"])));

    setProgress(60);

    //日志信息等级
    MyString logLevel[]={"用户","错误","警告","调试","临时"};
    for(int i=0;i<sizeof(logLevel)/sizeof(logLevel[0]);i++){
        cmbLogLevel->InsertString(i,logLevel[i]);
    }
    cmbLogLevel->SetCurSel(3);

    setProgress(70);

    //设置标题
    int BigVersion=Version/100;
    int SmlVersion=Version%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    setText(Dlg,getText(Dlg)+MyString::Format(" Ver%d.%d",BigVersion,SmlVersion));

    setProgress(80);

    //刷新通信方式
    updateCommunityWay();

    setProgress(90);

    //自动检查更新
    if(conf["AutoCheck"]==Stringify(true)){
        if(isConnectedNet()){
            if(NetGetVersion()>Version){
                MyLog::user("有可更新版本,在设置中更新.");
            }
        }
    }

    setProgress(100);
    MyLog::user("初始化完毕");
    MainDialogCtrlValidity::Init();
}

// 向MainDialog的消息处理函数发送消息
void sendMainDialogMessage(int Message){
    if(test.isTest(test.UI)){
        return;
    }
    SendMessage(hwnd,Message,Message,0);
}

// 用于存放当前串口情况
std::vector<int>* idle=new std::vector<int>();
// 用于存放之前的串口情况,可以实现简单的自动连接
std::vector<int>* lastIdle=NULL;

// 获得通信方式中选择的Com号,USB返回=0,串口返回>0
int getComID(){
    if(!cmbWay->GetCurSel()){
        return 0;
    }
    return (*idle)[idle->size()-cmbWay->GetCurSel()];
}

void setProgress(int percent){
    if(test.isTest(test.UI)){
        test.uiTest.UI_progress=percent;
        return;
    }
    if(percent<0){
        progress->SetPos(0);
    } else if(percent>100){
        progress->SetPos(100);
    } else{
        progress->SetPos(percent);
    }
}

void setProgress(double percent){
    setProgress((int)percent);
}

#pragma region 枚举串口的复杂代码

#include <initguid.h>
// 以下定义来自DDK中的<ntddser.h>,串行端口枚举也需要它
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT,0x86e0d1e0L,0x8089,0x11d0,0x9c,0xe4,0x08,0x00,0x3e,0x30,0x1f,0x73);
#endif
BOOL EnumPortsWdm(){
    cmbWay->ResetContent();

    GUID *guidDev=(GUID*)&GUID_CLASS_COMPORT;
    HDEVINFO hDevInfo=SetupDiGetClassDevs(guidDev,NULL,NULL,DIGCF_DEVICEINTERFACE);
    if(INVALID_HANDLE_VALUE==hDevInfo){
        return FALSE;
    }
    BOOL returnVal=TRUE;
    SP_DEVICE_INTERFACE_DATA ifcData;
    const int dwDetDataSize=1024;
    char __pDetData[dwDetDataSize];
    SP_DEVICE_INTERFACE_DETAIL_DATA* pDetData=(SP_DEVICE_INTERFACE_DETAIL_DATA*)__pDetData;
    ifcData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);
    pDetData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
    for(int i=0;;i++){
        if(!SetupDiEnumDeviceInterfaces(hDevInfo,NULL,guidDev,i,&ifcData)){
            returnVal=FALSE;
            break;
        }
        SP_DEVINFO_DATA devdata={sizeof(SP_DEVINFO_DATA)};
        if(!SetupDiGetDeviceInterfaceDetail(hDevInfo,&ifcData,pDetData,dwDetDataSize,NULL,&devdata)){
            returnVal=FALSE;
            break;
        }
        WCHAR fname[256]={0};
        if(!SetupDiGetDeviceRegistryProperty(
            hDevInfo,&devdata,SPDRP_FRIENDLYNAME,NULL,(PBYTE)fname,sizeof(fname),NULL)){
            //无法获得此设备友好名称
        }
        MyString friendlyName=fname;
        const char* str=friendlyName;
        for(str=str+strlen(str);*str!='(';str--);
        int COM=0;
        sscanf_s(str,"(COM%d)",&COM);

        HANDLE hPort=::CreateFile(MyString::Format("\\\\.\\COM%d",COM),
            GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
        if(hPort!=INVALID_HANDLE_VALUE){
            idle->push_back(COM);
            CloseHandle(hPort);
            cmbWay->InsertString(0,friendlyName);
        }
    }
    if(hDevInfo!=INVALID_HANDLE_VALUE){
        SetupDiDestroyDeviceInfoList(hDevInfo);
    }
    MyLog::user("发现%d个串口",idle->size());
    return returnVal;
}

#pragma endregion

// 更新通信方式组合框
void updateCommunityWay(){
    if(lastIdle){
        delete lastIdle;
    }
    lastIdle=idle;
    idle=new std::vector<int>();

    //清除串口数组内容
    idle->clear();
#define FastEnum
    //原来不能显示名字的枚举方式,代码没错,但是不够强:
#ifndef FastEnum
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
#else
    EnumPortsWdm();
#endif
#undef FastEnum
    //对USB的支持
    cmbWay->InsertString(0,_T("USB"));
}

// 尝试自动连接
void autoConnect(){
    int id=comm.getConnectId();
    //自动连接只在未连接状态生效
    if(id>0){
        if(idle->size()==lastIdle->size()+1){ //应该会增加一个空闲设备
            std::vector<int> diff;
            std::sort(idle->begin(),idle->end());
            std::sort(lastIdle->begin(),lastIdle->end());
            std::set_difference(
                idle->begin(),idle->end(),
                lastIdle->begin(),lastIdle->end(),
                std::inserter(diff,diff.begin())
            );//求idle-lastIdle
            if(diff.size()==1){ //差集应该只有一个元素
                int needConnectId=diff[0];
                for(uint i=0;i<idle->size();i++){
                    if((*idle)[i]==needConnectId){
                        cmbWay->SetCurSel(i+1);
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
                        error+=MyString::IntToMyString(*it);
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
                    error+=MyString::IntToMyString(*it);
                }
                MyLog::error(error);
            }
            {
                MyString error="lastIdle:";
                for(std::vector<int>::iterator it=lastIdle->begin();it!=lastIdle->end();it++){
                    error+=MyString::IntToMyString(*it);
                }
                MyLog::error(error);
            }
        }
    }
}

// 自动断开连接
void autoDisconnect(){
    int id=comm.getConnectId();
    //自动断开仅当连接状态下
    if(id>0){
        bool needDisconnect=true;
        for(uint i=0;i<idle->size();i++){
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
