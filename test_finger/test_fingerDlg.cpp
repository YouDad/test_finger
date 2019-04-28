#include "stdafx.h"
#include "afxdialogex.h"
#include "test_fingerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEdit* editLog;
CEdit* editAddress;
CEdit* editPassword;
CEdit* editAddressSet;
CEdit* editPasswordSet;
CEdit* editLightTime;
CEdit* editSensitivity;
CEdit* editReadRegAddr;
CEdit* editReadRegVal;
CEdit* editWriteRegAddr;
CEdit* editWriteRegVal;
CComboBox* cmbWay;
CComboBox* cmbBaud;
CComboBox* cmbBaudSet;
CComboBox* cmbChipType;
CComboBox* cmbLogLevel;
CButton* btnConnect;
CButton* btnRawImage;
CButton* btnTestImage;
CButton* btnContinueImage;
CButton* btnContinueBackGroundImage;
CButton* btnSetCmos;
CButton* btnSetBaud;
CButton* btnSetPassword;
CButton* btnSetAddress;
CButton* btnSaveLog;
CButton* btnReadReg;
CButton* btnWriteReg;
CButton* chkTimeoutContinue;
CStatic* textDevice;
CStatic* image;
CProgressCtrl* progress;
BYTE packet[65536];
DWORD packetCnt;
BYTE packetData[65536];
DWORD packetDataLen;
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
    SetIcon(m_hIcon,TRUE);			// 设置大图标
    SetIcon(m_hIcon,FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    ///1.全局控件赋值
    editLog=(CEdit*)GetDlgItem(IDC_EDITLog);
    editAddress=(CEdit*)GetDlgItem(IDC_EDITAddress);
    editPassword=(CEdit*)GetDlgItem(IDC_EDITPassword);
    editAddressSet=(CEdit*)GetDlgItem(IDC_EDITAddressSet);
    editPasswordSet=(CEdit*)GetDlgItem(IDC_EDITPasswordSet);
    editLightTime=(CEdit*)GetDlgItem(IDC_EDITLightTime);
    editSensitivity=(CEdit*)GetDlgItem(IDC_EDITSensitivity);
    editReadRegAddr=(CEdit*)GetDlgItem(IDC_EDITreadRegAddr);
    editReadRegVal=(CEdit*)GetDlgItem(IDC_EDITreadRegVal);
    editWriteRegAddr=(CEdit*)GetDlgItem(IDC_EDITwriteRegAddr);
    editWriteRegVal=(CEdit*)GetDlgItem(IDC_EDITwriteRegVal);
    cmbWay=(CComboBox*)GetDlgItem(IDC_CMBWay);
    cmbBaud=(CComboBox*)GetDlgItem(IDC_CMBBaud);
    cmbBaudSet=(CComboBox*)GetDlgItem(IDC_CMBBaudSet);
    cmbChipType=(CComboBox*)GetDlgItem(IDC_CMBChipType);
    cmbLogLevel=(CComboBox*)GetDlgItem(IDC_CMBLogLevel);
    btnConnect=(CButton*)GetDlgItem(IDC_BTNConnect);
    btnRawImage=(CButton*)GetDlgItem(IDC_BTNRawImage);
    btnTestImage=(CButton*)GetDlgItem(IDC_BTNTestImage);
    btnContinueImage=(CButton*)GetDlgItem(IDC_BTNContinueImage);
    btnContinueBackGroundImage=(CButton*)GetDlgItem(IDC_BTNContinueBackGroundImage);
    btnSetCmos=(CButton*)GetDlgItem(IDC_BTNSetCmos);
    btnSetBaud=(CButton*)GetDlgItem(IDC_BTNSetBaud);
    btnSetPassword=(CButton*)GetDlgItem(IDC_BTNSetPassword);
    btnSetAddress=(CButton*)GetDlgItem(IDC_BTNSetAddress);
    btnSaveLog=(CButton*)GetDlgItem(IDC_BTNSaveLog);
    btnReadReg=(CButton*)GetDlgItem(IDC_BTNreadReg);
    btnWriteReg=(CButton*)GetDlgItem(IDC_BTNwriteReg);
    chkTimeoutContinue=(CButton*)GetDlgItem(IDC_CHKTimeoutContinue);
    textDevice=(CStatic*)GetDlgItem(IDC_TEXTDevice);
    image=(CStatic*)GetDlgItem(IDC_IMAGE);
    progress=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS);

    ///2.对组合框的初始化
    updateCommunityWay();
    //通讯方式
    const WCHAR* way[]={0};
    for(int i=0;way[i];i++){
        cmbWay->InsertString(i,way[i]);
    }
    //常用波特率
    const WCHAR* baud[]={_T("9600"),_T("115200"),_T("256000"),0};
    for(int i=0;baud[i];i++){
        cmbBaud->InsertString(i,baud[i]);
        cmbBaudSet->InsertString(i,baud[i]);
    }
    cmbBaud->SetCurSel(1);
    //芯片类型
    const WCHAR* chipType[]={_T("默认"),_T("航芯"),0};
    for(int i=0;chipType[i];i++){
        cmbChipType->InsertString(i,chipType[i]);
    }
    cmbChipType->SetCurSel(0);
    //日志信息等级
    const WCHAR* logLevel[]={_T("用户"),_T("错误"),_T("警告"),_T("调试"),_T("临时"),0};
    for(int i=0;logLevel[i];i++){
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
    WCHAR str[64];
    btnConnect->GetWindowTextW(str,512);
    if(lstrcmp(str,_T("连接下位机"))==0){
        updateControlDisable(actOpeningPort);

        int com,baud=GetDlgItemInt(IDC_CMBBaud);//读取波特率
        cmbWay->GetWindowText(str,64);
        swscanf(str,_T("COM%d"),&com);//读取通信方式

        bool ret=comm.connect(com,baud);
        if(ret){
            btnConnect->SetWindowText(_T("断开连接"));
            updateControlDisable(actOpenedPort);
        } else{
            updateControlDisable(actClosedPort);
        }
    } else{
        comm.disconnect();
        updateControlDisable(actClosedPort);
        btnConnect->SetWindowText(_T("连接下位机"));
    }
}

//日志保存按钮点击事件
void Ctest_fingerDlg::OnBnClickedBtnsavelog(){
    //获取文件路径名
    LPCTSTR filter=_T("文本文件(*.txt)|*.txt||");
    CFileDialog dlgFileOpen(0,0,0,0,filter,0);
    if(dlgFileOpen.DoModal()==IDOK){
        CString path=dlgFileOpen.GetPathName();
        //get text
        CString logText;
        editLog->GetWindowText(logText);
        //Add .txt ?
        if(path.Find(_T(".txt"),path.GetLength()-4)==-1){
            path=path+_T(".txt");
        }
        //CString->char*
        char* filePath=CString2char(path);
        //write into file
        FILE* fp=fopen(filePath,"w");
        fprintf_s(fp,"%s",CString2char(logText));
        fclose(fp);
    }
}

bool timeoutThread_continueImage;

MyThread ImageTimeout(ThreadFunction__(timeoutFunction)(void){
    Sleep(10*1000);
    if(!timeoutThread_continueImage){
        MyLog.print(Log::LOGU,"采图超时");
        updateControlDisable(actGotImage);
    }
    return;
});

bool timeoutThread_Register;

MyThread RegisterTimeout(ThreadFunction__(timeoutFunction)(void){
    timeoutThread_Register=false;
    Sleep(1*1000);
    if(!timeoutThread_Register){
        MyLog.print(Log::LOGU,"读寄存器超时");
        updateControlDisable(actReadedReg);
    }
});

//原始图像的点击事件
void Ctest_fingerDlg::OnBnClickedBtnrawimage(){
    updateControlDisable(actGetingImage);
    progress->SetPos(10);
    MyLog.print(Log::LOGD,"Main Thread:1开始采集原始图像");
    progress->SetPos(20);
    comm.request(CMD_GET_RAW_IMAGE);
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"Serial Thread:3向下位机发送命令:CMD_GET_RAW_IMAGE");
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
            timeoutThread_continueImage=true;
        }
        case WM_GET_RAW_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(CMD_GET_RAW_IMAGE);
                ImageTimeout.start();
            } else{
                updateControlDisable(actGotImage);
            }
        }break;
        case WM_STP_GET_IMAGE:
        {
            continueImage=false;
            timeoutThread_continueImage=false;
            updateControlDisable(actGotImage);
            ImageTimeout.terminate();
        }break;
        case WM_READ_REGISTER:
        {
            timeoutThread_Register=true;
        }break;
        case WM_WRITE_REGISTER:
        {

        }break;
        case WM_GET_CON_BKI:
        {
            continueImage=true;
            timeoutThread_continueImage=true;
        }
        case WM_GET_TEST_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(CMD_GET_TEST_IMAGE);
                ImageTimeout.start();
            } else{
                updateControlDisable(actGotImage);
            }
        }break;
        case WM_STP_GET_BKI:
        {
            continueImage=false;
            timeoutThread_continueImage=false;
            updateControlDisable(actGotImage);
            ImageTimeout.terminate();
        }break;
    }
    progress->SetPos(0);
    return 1;
}

//连续获取图像的点击事件
void Ctest_fingerDlg::OnBnClickedBtncontinueimage(){
    //根据按钮上的文字判断当前连接状态
    WCHAR str[512];
    btnContinueImage->GetWindowTextW(str,512);
    if(lstrcmp(str,_T("连续获取图像"))==0){
        MyLog.print(Log::LOGU,"开始连续获取图像");
        updateControlDisable(actGetConImage);
        btnContinueImage->SetWindowText(_T("停止获取图像"));
        SendMessage(WM_GET_CON_IMAGE,WM_GET_CON_IMAGE,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取图像");
        updateControlDisable(actStpGetImage);
        btnContinueImage->SetWindowText(_T("连续获取图像"));
        SendMessage(WM_STP_GET_IMAGE,WM_STP_GET_IMAGE,0);
    }
}


void Ctest_fingerDlg::OnBnClickedBtndevlog(){
    MyLog.print(Log::LOGU,"V0.9 <时间未知>:完成了串口连接和图片显示,完成了日志功能的建设");
    MyLog.print(Log::LOGU,"V1.0 <2019年3月16日15:36:11>:完成原始图像和连续取图按钮功能");
    MyLog.print(Log::LOGU,"V1.1 <2019年3月16日15:54:23>:完成按钮互斥,防止线程冲突,添加开发日志");
    MyLog.print(Log::LOGU,"V1.2 <2019年3月18日00:57:44>:添加读写寄存器,添加进度条,添加选图像大小");
    MyLog.print(Log::LOGU,"V1.3 <2019年3月24日13:59:42>:完成了无用功能删减,放大了指纹图像,修复了按钮互斥bug");
    MyLog.print(Log::LOGU,"V1.4 <2019年3月24日14:12:08>:添加了在release模式下取消warning的代码,添加了打开文件夹按钮");
    MyLog.print(Log::LOGU,"V1.5 <2019年4月10日17:15:45>:完成连续取背景功能");
    MyLog.print(Log::LOGU,"V1.6 <2019年4月22日22:42:59>:适配了航芯的取原始图像功能,修复了时间和编辑框鬼畜bug");
    MyLog.print(Log::LOGU,"V2.0 <2019年4月28日17:17:46>:升级了串口架构,支持主动和被动模式并存");
}

void Ctest_fingerDlg::OnBnClickedBtnreadreg(){
    updateControlDisable(actReadingReg);
    progress->SetPos(10);
    MyLog.print(Log::LOGD,"Main Thread:1开始读寄存器");

    CString hex;
    int integer;
    editReadRegAddr->GetWindowText(hex);
    sscanf(CString2char(hex),"%X",&integer);
    uint8_t address=integer;

    comm.request(CMD_READ_NOTE_BOOK,&address,1);

    progress->SetPos(20);
    RegisterTimeout.start();
}


void Ctest_fingerDlg::OnBnClickedBtnwritereg(){
    updateControlDisable(actWritingReg);
    progress->SetPos(10);
    MyLog.print(Log::LOGD,"Main Thread:1开始写寄存器");

    CString hex;
    int integer;
    uint8_t addrVal[2];
    editWriteRegAddr->GetWindowText(hex);
    sscanf(CString2char(hex),"%X",&integer);
    addrVal[0]=integer;
    editWriteRegVal->GetWindowText(hex);
    sscanf(CString2char(hex),"%X",&integer);
    addrVal[1]=integer;

    comm.request(CMD_WRITE_NOTE_BOOK,addrVal,2);
    SendMessage(WM_WRITE_REGISTER,WM_WRITE_REGISTER,0);

    progress->SetPos(20);

    updateControlDisable(actWritedReg);
}


void Ctest_fingerDlg::OnBnClickedBtnsetcmos(){
    WCHAR a[100],b[100];
    GetDlgItemText(IDC_EDITLightTime,a,100);
    GetDlgItemText(IDC_EDITSensitivity,b,100);
    if(a[0]==0&&b[0]==0)
        MyLog.print(Log::LOGU,"设置失败,两个均为空");
    else if(a[0]==0)
        MyLog.print(Log::LOGU,CString(_T("设置灵敏度为"))+b);
    else if(b[0]==0)
        MyLog.print(Log::LOGU,CString(_T("设置曝光时间为"))+a);
    else
        MyLog.print(Log::LOGU,CString(_T("设置曝光时间为"))+a+_T(" , ")+_T("设置灵敏度为")+b);
}


void Ctest_fingerDlg::OnBnClickedBtnsetbaud(){
    int baud=GetDlgItemInt(IDC_CMBBaudSet);
    MyLog.print(Log::LOGU,"设置串口波特率为%d",baud);
}


void Ctest_fingerDlg::OnBnClickedBtnsetpassword(){
    WCHAR str[100];
    GetDlgItemText(IDC_EDITPasswordSet,str,100);
    MyLog.print(Log::LOGU,CString(_T("设置串口通信密码为"))+str);
}


void Ctest_fingerDlg::OnBnClickedBtnsetaddress(){
    WCHAR str[100];
    GetDlgItemText(IDC_EDITAddressSet,str,100);
    MyLog.print(Log::LOGU,CString(_T("设置串口通信地址为"))+str);
}


void Ctest_fingerDlg::OnBnClickedBtncontinuebackgroundimage(){
    //根据按钮上的文字判断当前连接状态
    WCHAR str[512];
    btnContinueBackGroundImage->GetWindowTextW(str,512);
    if(lstrcmp(str,_T("连续获取背景"))==0){
        MyLog.print(Log::LOGU,"开始连续获取背景");
        updateControlDisable(actGetConBKI);
        btnContinueBackGroundImage->SetWindowText(_T("停止获取背景"));
        SendMessage(WM_GET_CON_BKI,WM_GET_CON_BKI,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取背景");
        updateControlDisable(actStpGetBKI);
        btnContinueBackGroundImage->SetWindowText(_T("连续获取背景"));
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
    progress->SetPos(10);
    MyLog.print(Log::LOGD,"Main Thread:开始采集背景");
    comm.request(CMD_GET_TEST_IMAGE);
    progress->SetPos(20);
}
