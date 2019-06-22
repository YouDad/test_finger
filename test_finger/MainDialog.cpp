#include "stdafx.h"

// 主窗口的构造函数
MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(MainDialog::IDD,pParent){
    m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// 消息映射宏处理块
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
    ON_BN_CLICKED(IDC_BTNConnect,&MainDialog::OnBnClickedBtnConnect)
    ON_BN_CLICKED(IDC_BTNSaveLog,&MainDialog::OnBnClickedBtnSaveLog)
    ON_BN_CLICKED(IDC_BTNRawImage,&MainDialog::OnBnClickedBtnRawImage)
    ON_BN_CLICKED(IDC_BTNContinueImage,&MainDialog::OnBnClickedBtnContinueImage)
    ON_BN_CLICKED(IDC_BTNdevLog,&MainDialog::OnBnClickedBtnDevLog)
    ON_BN_CLICKED(IDC_BTNreadReg,&MainDialog::OnBnClickedBtnReadReg)
    ON_BN_CLICKED(IDC_BTNwriteReg,&MainDialog::OnBnClickedBtnWriteReg)
    ON_BN_CLICKED(IDC_BTNOpenImage,&MainDialog::OnBnClickedBtnOpenImage)
    ON_BN_CLICKED(IDC_BTNContinueBGImg,&MainDialog::OnBnClickedBtnContinueBackgroundImage)
    ON_BN_CLICKED(IDC_BTNOpenBackGroundImage,&MainDialog::OnBnClickedBtnOpenBackgroundImage)
    ON_BN_CLICKED(IDC_BTNTestImage,&MainDialog::OnBnClickedBtnBackgroundImage)
    ON_CBN_CLOSEUP(IDC_CMBLogLevel,&MainDialog::OnCbnCloseupCmbLogLevel)
    ON_BN_CLICKED(IDC_BTNAdvDbg,&MainDialog::OnBnClickedBtnAdvDbg)
    ON_BN_CLICKED(IDC_BTN0,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN1,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN2,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN3,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN4,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN5,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN6,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN7,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTNEnroll,&MainDialog::OnBnClickedBtnEnroll)
    ON_BN_CLICKED(IDC_BTNMatch,&MainDialog::OnBnClickedBtnMatch)
    ON_BN_CLICKED(IDC_BTNDeviceInfo,&MainDialog::OnBnClickedBtnDeviceInfo)
    ON_BN_CLICKED(IDC_BTNViewEnrollIds,&MainDialog::OnBnClickedBtnViewEnrollIDs)
    ON_BN_CLICKED(IDC_BTNDeleteTemplate,&MainDialog::OnBnClickedBtnDeleteTemplate)
    ON_BN_CLICKED(IDC_BTNCancel,&MainDialog::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_BTNClearLog,&MainDialog::OnBnClickedBtnClearLog)
    ON_BN_CLICKED(IDC_BTNSetting,&MainDialog::OnBnClickedBtnSetting)
    ON_CBN_SELCHANGE(IDC_CMBBaud,&MainDialog::OnCbnSelChangeCmbBaud)
    ON_CBN_SELCHANGE(IDC_CMBProtocolType,&MainDialog::OnCbnSelChangeCmbProtocolType)
    ON_CBN_SELCHANGE(IDC_CMBWay,&MainDialog::OnCbnSelChangeCmbWay)
END_MESSAGE_MAP()

// 重写Enter事件,阻止Enter默认行为:关闭窗口
void MainDialog::OnOK(){}

// 初始化窗口事件
BOOL MainDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon,TRUE);
    SetIcon(m_hIcon,FALSE);

    (new MyThread([&](){initMainControl(this);},true))->start();

    return TRUE;//除非将焦点设置到控件，否则返回 TRUE
}

// 重画事件
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

// 查询图标事件
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

//串口线程消息处理函数
LRESULT MainDialog::serialResponse(WPARAM w,LPARAM l){
    static bool continueImage=false;
    switch(w){
    case WM_GET_CON_IMAGE:
    {
        continueImage=true;
    }// break; 没有break
    case WM_GET_RAW_IMAGE:
    {
        if(continueImage){
            comm.request(SII(GetRawImage));
            setProgress(30);
            MyLog::user("请放手指");
        } else{
            MainDialogCtrlValidity::Work();
        }
    }break;
    case WM_STP_GET_IMAGE:
    {
        continueImage=false;
        MainDialogCtrlValidity::Work();
        setProgress(0);
    }break;
    case WM_READ_REGISTER:
    {
        MainDialogCtrlValidity::Work();
        setProgress(100);
    }break;
    case WM_APPEND_CONTROLS:
    {
        advancedDebugDialog->append();
    }break;
    case WM_GET_CON_BKI:
    {
        continueImage=true;
    }// break; 没有break
    case WM_GET_TEST_IMAGE:
    {
        if(continueImage){
            comm.request(SII(GetTestImage));
            setProgress(30);
        } else{
            MainDialogCtrlValidity::Work();
        }
    }break;
    case WM_STP_GET_BKI:
    {
        continueImage=false;
        MainDialogCtrlValidity::Work();
        setProgress(0);
    }break;
    }
    setProgress(0);
    return TRUE;
}
