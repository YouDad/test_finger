#include "stdafx.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND hwnd;
// Ctest_fingerDlg 对话框

Ctest_fingerDlg::Ctest_fingerDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(Ctest_fingerDlg::IDD,pParent){
    m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_fingerDlg::DoDataExchange(CDataExchange* pDX){
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_fingerDlg,CDialogEx)
    ON_WM_PAINT()
    ON_MESSAGE(WM_GET_RAW_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_IMAGE,serialResponse)
    ON_MESSAGE(WM_STP_GET_IMAGE,serialResponse)
    ON_MESSAGE(WM_READ_REGISTER,serialResponse)
    ON_MESSAGE(WM_WRITE_REGISTER,serialResponse)
    ON_MESSAGE(WM_GET_TEST_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_BKI,serialResponse)
    ON_MESSAGE(WM_STP_GET_BKI,serialResponse)
    ON_WM_QUERYDRAGICON()
    ON_WM_DEVICECHANGE()
    ON_BN_CLICKED(IDC_BTNConnect,&Ctest_fingerDlg::OnBnClickedBtnconnect)
    ON_BN_CLICKED(IDC_BTNSaveLog,&Ctest_fingerDlg::OnBnClickedBtnsavelog)
    ON_BN_CLICKED(IDC_BTNRawImage,&Ctest_fingerDlg::OnBnClickedBtnrawimage)
    ON_BN_CLICKED(IDC_BTNContinueImage,&Ctest_fingerDlg::OnBnClickedBtncontinueimage)
    ON_BN_CLICKED(IDC_BTNdevLog,&Ctest_fingerDlg::OnBnClickedBtndevlog)
    ON_BN_CLICKED(IDC_BTNreadReg,&Ctest_fingerDlg::OnBnClickedBtnreadreg)
    ON_BN_CLICKED(IDC_BTNwriteReg,&Ctest_fingerDlg::OnBnClickedBtnwritereg)
    ON_BN_CLICKED(IDC_BTNSetCmos,&Ctest_fingerDlg::OnBnClickedBtnsetcmos)
    ON_BN_CLICKED(IDC_BTNSetBaud,&Ctest_fingerDlg::OnBnClickedBtnsetbaud)
    ON_BN_CLICKED(IDC_BTNSetPassword,&Ctest_fingerDlg::OnBnClickedBtnsetpassword)
    ON_BN_CLICKED(IDC_BTNSetAddress,&Ctest_fingerDlg::OnBnClickedBtnsetaddress)
    ON_BN_CLICKED(IDC_BTNOpenImage,&Ctest_fingerDlg::OnBnClickedBtnopenimage)
    ON_BN_CLICKED(IDC_BTNContinueBackGroundImage,&Ctest_fingerDlg::OnBnClickedBtncontinuebackgroundimage)
    ON_BN_CLICKED(IDC_BTNOpenBackGroundImage,&Ctest_fingerDlg::OnBnClickedBtnopenbackgroundimage)
    ON_BN_CLICKED(IDC_BTNTestImage,&Ctest_fingerDlg::OnBnClickedBtnbackgroundimage)
END_MESSAGE_MAP()


// Ctest_fingerDlg 消息处理程序

BOOL Ctest_fingerDlg::OnInitDialog(){
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon,TRUE);// 设置大图标
    SetIcon(m_hIcon,FALSE);// 设置小图标

    // TODO: 在此添加额外的初始化代码

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

    ///4.线程句柄清零
    serialThread=0;

    ///5.进度条设置
    progress->SetRange(0,100);
    progress->SetPos(0);

    hwnd=m_hWnd;

    ///0.测试

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//如果向对话框添加最小化按钮，则需要下面的代码
//来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//这将由框架自动完成。
void Ctest_fingerDlg::OnPaint(){
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR Ctest_fingerDlg::OnQueryDragIcon(){
    return static_cast<HCURSOR>(m_hIcon);
}

//支持串口热拔插的设备更改监听函数
BOOL Ctest_fingerDlg::OnDeviceChange(UINT nEventType,DWORD dwData){
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
void Ctest_fingerDlg::OnBnClickedBtnconnect(){
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
void Ctest_fingerDlg::OnBnClickedBtnsavelog(){
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
void Ctest_fingerDlg::OnBnClickedBtnrawimage(){
    updateControlDisable(actGetingImage);
    comm.request(CMD_GET_RAW_IMAGE);
    progress->SetPos(30);
    MyLog.print(Log::LOGU,"请放手指");
    ImageTimeout.start();
}

//串口线程消息处理函数
LRESULT Ctest_fingerDlg::serialResponse(WPARAM w,LPARAM l){
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
                comm.request(CMD_GET_RAW_IMAGE);//line 289 重合
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
        case WM_WRITE_REGISTER:
        {
            progress->SetPos(100);
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
void Ctest_fingerDlg::OnBnClickedBtncontinueimage(){
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


void Ctest_fingerDlg::OnBnClickedBtndevlog(){
    MyLog.DevelopLog();
}

void Ctest_fingerDlg::OnBnClickedBtnreadreg(){
    updateControlDisable(actReadingReg);
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始读寄存器");

    uint8_t address=getHex(editReadRegAddr);

    comm.request(CMD_READ_NOTE_BOOK,&address,1);

    progress->SetPos(60);
    RegisterTimeout.start();
}


void Ctest_fingerDlg::OnBnClickedBtnwritereg(){
    updateControlDisable(actWritingReg);
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"开始写寄存器");

    uint8_t addrVal[2];
    addrVal[0]=getHex(editWriteRegAddr);
    addrVal[1]=getHex(editWriteRegVal);

    comm.request(CMD_WRITE_NOTE_BOOK,addrVal,2);
    SendMessage(WM_WRITE_REGISTER,WM_WRITE_REGISTER,0);

    progress->SetPos(100);
    updateControlDisable(actWritedReg);
}


void Ctest_fingerDlg::OnBnClickedBtnsetcmos(){
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


void Ctest_fingerDlg::OnBnClickedBtnsetbaud(){
    MyLog.print(Log::LOGU,MyString("设置串口通信密码为")+getText(cmbBaudSet));
    progress->SetPos(100);
}


void Ctest_fingerDlg::OnBnClickedBtnsetpassword(){
    MyLog.print(Log::LOGU,MyString("设置串口通信密码为")+getText(editPasswordSet));
    progress->SetPos(100);
}


void Ctest_fingerDlg::OnBnClickedBtnsetaddress(){
    MyLog.print(Log::LOGU,MyString("设置串口通信地址为")+getText(editAddressSet));
    progress->SetPos(100);
}


void Ctest_fingerDlg::OnBnClickedBtncontinuebackgroundimage(){
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


void Ctest_fingerDlg::OnBnClickedBtnopenimage(){
    if(access("collectedImage",0)){
        MyLog.print(Log::LOGU,"图片文件夹不存在,请先采一张图片");
    } else{
        ShellExecuteA(NULL,"explore","collectedImage",NULL,NULL,SW_NORMAL);
    }
}


void Ctest_fingerDlg::OnBnClickedBtnopenbackgroundimage(){
    if(access("collectedBGI",0)){
        MyLog.print(Log::LOGU,"背景文件夹不存在,请先采一张背景");
    } else{
        ShellExecuteA(NULL,"explore","collectedBGI",NULL,NULL,SW_NORMAL);
    }
}


void Ctest_fingerDlg::OnBnClickedBtnbackgroundimage(){
    updateControlDisable(actGetingImage);
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始采集背景");
    comm.request(CMD_GET_TEST_IMAGE);
    progress->SetPos(60);
}
