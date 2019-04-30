#include "stdafx.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(MainDialog::IDD,pParent){
    m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(MainDialog,CDialogEx)
    ON_WM_PAINT()
    ON_MESSAGE(WM_GET_RAW_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_IMAGE,serialResponse)
    ON_MESSAGE(WM_STP_GET_IMAGE,serialResponse)
    ON_MESSAGE(WM_READ_REGISTER,serialResponse)
    ON_MESSAGE(WM_APPEND_CONTROLS,serialResponse)
    ON_MESSAGE(WM_GET_TEST_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_BKI,serialResponse)
    ON_MESSAGE(WM_STP_GET_BKI,serialResponse)
    ON_WM_QUERYDRAGICON()
    ON_WM_DEVICECHANGE()
    ON_BN_CLICKED(IDC_BTNConnect,&MainDialog::OnBnClickedBtnconnect)
    ON_BN_CLICKED(IDC_BTNSaveLog,&MainDialog::OnBnClickedBtnsavelog)
    ON_BN_CLICKED(IDC_BTNRawImage,&MainDialog::OnBnClickedBtnrawimage)
    ON_BN_CLICKED(IDC_BTNContinueImage,&MainDialog::OnBnClickedBtncontinueimage)
    ON_BN_CLICKED(IDC_BTNdevLog,&MainDialog::OnBnClickedBtndevlog)
    ON_BN_CLICKED(IDC_BTNreadReg,&MainDialog::OnBnClickedBtnreadreg)
    ON_BN_CLICKED(IDC_BTNwriteReg,&MainDialog::OnBnClickedBtnwritereg)
    ON_BN_CLICKED(IDC_BTNSetCmos,&MainDialog::OnBnClickedBtnsetcmos)
    ON_BN_CLICKED(IDC_BTNSetBaud,&MainDialog::OnBnClickedBtnsetbaud)
    ON_BN_CLICKED(IDC_BTNSetPassword,&MainDialog::OnBnClickedBtnsetpassword)
    ON_BN_CLICKED(IDC_BTNSetAddress,&MainDialog::OnBnClickedBtnsetaddress)
    ON_BN_CLICKED(IDC_BTNOpenImage,&MainDialog::OnBnClickedBtnopenimage)
    ON_BN_CLICKED(IDC_BTNContinueBackGroundImage,&MainDialog::OnBnClickedBtncontinuebackgroundimage)
    ON_BN_CLICKED(IDC_BTNOpenBackGroundImage,&MainDialog::OnBnClickedBtnopenbackgroundimage)
    ON_BN_CLICKED(IDC_BTNTestImage,&MainDialog::OnBnClickedBtnbackgroundimage)
    ON_CBN_CLOSEUP(IDC_CMBLogLevel,&MainDialog::OnCbnCloseupCmbloglevel)
    ON_BN_CLICKED(IDC_BTNAdvDbg,&MainDialog::OnBnClickedBtnadvdbg)
END_MESSAGE_MAP()

void MainDialog::OnOK(){}

BOOL MainDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon,TRUE);
    SetIcon(m_hIcon,FALSE);

    ///1.全局控件赋值
    initMyControl(this);

    ///2.对组合框的初始化
    updateCommunityWay();

    ////通讯方式
    //const WCHAR* way[]={0};
    //for(int i=0;way[i];i++){
    //    cmbWay->InsertString(i,way[i]);
    //}

    //常用波特率
    MyString baud[]={"9600","115200","256000"};
    for(int i=0;i<3;i++){
        cmbBaud->InsertString(i,baud[i]);
        cmbBaudSet->InsertString(i,baud[i]);
    }
    cmbBaud->SetCurSel(1);

    ////芯片类型
    //const WCHAR* chipType[]={_T("默认"),_T("航芯"),0};
    //for(int i=0;chipType[i];i++){
    //    cmbChipType->InsertString(i,chipType[i]);
    //}
    //cmbChipType->SetCurSel(0);

    //日志信息等级
    MyString logLevel[]={"用户","错误","警告","调试","临时"};
    for(int i=0;i<5;i++){
        cmbLogLevel->InsertString(i,logLevel[i]);
    }
    cmbLogLevel->SetCurSel(3);

    ///3.更新 各控件访问权限
    updateControlDisable(actInit);

    ///5.进度条设置
    progress->SetRange(0,100);
    progress->SetPos(0);

    return TRUE;//除非将焦点设置到控件，否则返回 TRUE
}

void MainDialog::OnPaint(){
    if(IsIconic()){
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);
        int cxIcon=GetSystemMetrics(SM_CXICON);
        int cyIcon=GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x=(rect.Width()-cxIcon+1)/2;
        int y=(rect.Height()-cyIcon+1)/2;
        dc.DrawIcon(x,y,m_hIcon);
    } else{
        CDialogEx::OnPaint();
    }
}

HCURSOR MainDialog::OnQueryDragIcon(){
    return static_cast<HCURSOR>(m_hIcon);
}

//支持串口热拔插的设备更改监听函数
BOOL MainDialog::OnDeviceChange(UINT nEventType,DWORD dwData){
    switch(nEventType){
        case DBT_DEVICEREMOVECOMPLETE://移除设备
            updateCommunityWay();
            autoDisconnect();
            break;
        case DBT_DEVICEARRIVAL://添加设备
            updateCommunityWay();
            autoConnect();
            break;
    }
    return TRUE;
}

//连接下位机按钮的点击事件
void MainDialog::OnBnClickedBtnconnect(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnConnect)=="连接下位机"){
        updateControlDisable(actOpeningPort);

        int com,baud=getInt(cmbBaud);//读取波特率
        sscanf(getText(cmbWay),"COM%d",&com);//读取通信方式

        bool ret=comm.connect(com,baud);
        if(ret){
            setText(btnConnect,"断开连接");
            updateControlDisable(actOpenedPort);
        } else{
            updateControlDisable(actClosedPort);
        }
    } else{
        comm.disconnect();
        updateControlDisable(actClosedPort);
        setText(btnConnect,"连接下位机");
    }
}

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

HANDLE timeoutThread_continueImage_Mutex=CreateMutex(0,0,0);
bool timeoutThread_continueImage;

MyThread ImageTimeout(ThreadFunction__(timeoutFunction)(void){
    Sleep(10*1000);
    WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
    if(!timeoutThread_continueImage){
        MyLog.print(Log::LOGU,"采图超时");
        updateControlDisable(actGotImage);
    }
    ReleaseMutex(timeoutThread_continueImage_Mutex);
});


MyThread RegisterTimeout(ThreadFunction__(timeoutFunction)(void){
    Sleep(1*1000);
    MyLog.print(Log::LOGU,"读寄存器超时");
    updateControlDisable(actReadedReg);
});

//原始图像的点击事件
void MainDialog::OnBnClickedBtnrawimage(){
    updateControlDisable(actGetingImage);
    comm.request(CMD_GET_RAW_IMAGE);
    progress->SetPos(30);
    MyLog.print(Log::LOGU,"请放手指");
    ImageTimeout.start();
}

//串口线程消息处理函数
LRESULT MainDialog::serialResponse(WPARAM w,LPARAM l){
    static bool continueImage=false;
    switch(w){
        case WM_GET_CON_IMAGE:
        {
            continueImage=true;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=true;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
        }
        case WM_GET_RAW_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(CMD_GET_RAW_IMAGE);
                progress->SetPos(30);
                MyLog.print(Log::LOGU,"请放手指");
                ImageTimeout.start();
            } else{
                updateControlDisable(actGotImage);
            }
        }break;
        case WM_STP_GET_IMAGE:
        {
            continueImage=false;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=false;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
            updateControlDisable(actGotImage);
            ImageTimeout.terminate();
            progress->SetPos(0);
        }break;
        case WM_READ_REGISTER:
        {
            RegisterTimeout.terminate();
            progress->SetPos(100);
        }break;
        case WM_APPEND_CONTROLS:
        {
            advancedDebugDialog->append(MyString::ParseInt($::conf["id"]),$::conf["AdvDbg_ImgId"]);
            $::conf["AdvDbg_ImgId"]=MyString::IntToMyString(MyString::ParseInt($::conf["AdvDbg_ImgId"])+1);
        }break;
        case WM_GET_CON_BKI:
        {
            continueImage=true;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=true;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
        }
        case WM_GET_TEST_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(CMD_GET_TEST_IMAGE);
                progress->SetPos(30);
                ImageTimeout.start();
            } else{
                updateControlDisable(actGotImage);
            }
        }break;
        case WM_STP_GET_BKI:
        {
            continueImage=false;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=false;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
            updateControlDisable(actGotImage);
            ImageTimeout.terminate();
            progress->SetPos(0);
        }break;
    }
    progress->SetPos(0);
    return 1;
}

//连续获取图像的点击事件
void MainDialog::OnBnClickedBtncontinueimage(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueImage)=="连续获取图像"){
        MyLog.print(Log::LOGU,"开始连续获取图像");
        updateControlDisable(actGetConImage);
        setText(btnContinueImage,"停止获取图像");
        SendMessage(WM_GET_CON_IMAGE,WM_GET_CON_IMAGE,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取图像");
        updateControlDisable(actStpGetImage);
        setText(btnContinueImage,"连续获取图像");
        SendMessage(WM_STP_GET_IMAGE,WM_STP_GET_IMAGE,0);
    }
}


void MainDialog::OnBnClickedBtndevlog(){
    MyLog.DevelopLog();
}

void MainDialog::OnBnClickedBtnreadreg(){
    updateControlDisable(actReadingReg);
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始读寄存器");

    uint8_t address=getHex(editReadRegAddr);

    comm.request(CMD_READ_NOTE_BOOK,&address,1);

    progress->SetPos(60);
    RegisterTimeout.start();
}


void MainDialog::OnBnClickedBtnwritereg(){
    updateControlDisable(actWritingReg);
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"开始写寄存器");

    uint8_t addrVal[2];
    addrVal[0]=getHex(editWriteRegAddr);
    addrVal[1]=getHex(editWriteRegVal);

    comm.request(CMD_WRITE_NOTE_BOOK,addrVal,2);

    progress->SetPos(100);
    updateControlDisable(actWritedReg);
}


void MainDialog::OnBnClickedBtnsetcmos(){
    MyString a=getText(editLightTime);
    if(a.length())
        a=MyString("设置曝光时间为")+a;
    MyString b=getText(editSensitivity);
    if(b.length())
        b=MyString("设置灵敏度为")+b;
    if(a.length()&&b.length())
        MyLog.print(Log::LOGU,a+b);
    else if(a.length())
        MyLog.print(Log::LOGU,a);
    else if(b.length())
        MyLog.print(Log::LOGU,b);
    else
        MyLog.print(Log::LOGU,"设置失败,两个均为空");
    progress->SetPos(100);
}


void MainDialog::OnBnClickedBtnsetbaud(){
    MyLog.print(Log::LOGU,MyString("设置串口通信密码为")+getText(cmbBaudSet));
    progress->SetPos(100);
}


void MainDialog::OnBnClickedBtnsetpassword(){
    MyLog.print(Log::LOGU,MyString("设置串口通信密码为")+getText(editPasswordSet));
    progress->SetPos(100);
}


void MainDialog::OnBnClickedBtnsetaddress(){
    MyLog.print(Log::LOGU,MyString("设置串口通信地址为")+getText(editAddressSet));
    progress->SetPos(100);
}


void MainDialog::OnBnClickedBtncontinuebackgroundimage(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueBackGroundImage)=="连续获取背景"){
        MyLog.print(Log::LOGU,"开始连续获取背景");
        updateControlDisable(actGetConBKI);
        setText(btnContinueBackGroundImage,"停止获取背景");
        SendMessage(WM_GET_CON_BKI,WM_GET_CON_BKI,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取背景");
        updateControlDisable(actStpGetBKI);
        setText(btnContinueBackGroundImage,"连续获取背景");
        SendMessage(WM_STP_GET_BKI,WM_STP_GET_BKI,0);
    }
}


void MainDialog::OnBnClickedBtnopenimage(){
    if(access("collectedImage",0)){
        MyLog.print(Log::LOGU,"图片文件夹不存在,请先采一张图片");
    } else{
        ShellExecuteA(NULL,"explore","collectedImage",NULL,NULL,SW_NORMAL);
    }
}


void MainDialog::OnBnClickedBtnopenbackgroundimage(){
    if(access("collectedBGI",0)){
        MyLog.print(Log::LOGU,"背景文件夹不存在,请先采一张背景");
    } else{
        ShellExecuteA(NULL,"explore","collectedBGI",NULL,NULL,SW_NORMAL);
    }
}


void MainDialog::OnBnClickedBtnbackgroundimage(){
    updateControlDisable(actGetingImage);
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始采集背景");
    comm.request(CMD_GET_TEST_IMAGE);
    progress->SetPos(60);
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
                $::conf["AdvDbg"]="true";
                btnAdvDbg->ShowWindow(SW_SHOW);
            }
        }
    }
}


void MainDialog::OnBnClickedBtnadvdbg(){
    if(advancedDebugDialog){
        delete advancedDebugDialog;
        advancedDebugDialog=0;
    } else{
        advancedDebugDialog=new AdvancedDebugDialog();
        advancedDebugDialog->Create(IDD_ADVANCED_DEBUG_DIALOG,this);
        advancedDebugDialog->ShowWindow(SW_SHOW);
    }
}
