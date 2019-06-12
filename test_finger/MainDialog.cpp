#include "stdafx.h"

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
    ON_BN_CLICKED(IDC_BTNOpenImage,&MainDialog::OnBnClickedBtnopenimage)
    ON_BN_CLICKED(IDC_BTNContinueBackGroundImage,&MainDialog::OnBnClickedBtncontinuebackgroundimage)
    ON_BN_CLICKED(IDC_BTNOpenBackGroundImage,&MainDialog::OnBnClickedBtnopenbackgroundimage)
    ON_BN_CLICKED(IDC_BTNTestImage,&MainDialog::OnBnClickedBtnbackgroundimage)
    ON_CBN_CLOSEUP(IDC_CMBLogLevel,&MainDialog::OnCbnCloseupCmbloglevel)
    ON_BN_CLICKED(IDC_BTNAdvDbg,&MainDialog::OnBnClickedBtnadvdbg)
    ON_BN_CLICKED(IDC_BTN0,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN1,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN2,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN3,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN4,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN5,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN6,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN7,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTNEnroll,&MainDialog::OnBnClickedBtnenroll)
    ON_BN_CLICKED(IDC_BTNMatch,&MainDialog::OnBnClickedBtnmatch)
    ON_BN_CLICKED(IDC_BTNDeviceInfo,&MainDialog::OnBnClickedBtndeviceinfo)
    ON_BN_CLICKED(IDC_BTNViewEnrollIds,&MainDialog::OnBnClickedBtnviewenrollids)
    ON_BN_CLICKED(IDC_BTNDeleteTemplate,&MainDialog::OnBnClickedBtndeletetemplate)
    ON_BN_CLICKED(IDC_BTNCancel,&MainDialog::OnBnClickedBtncancel)
    ON_BN_CLICKED(IDC_BTNClearLog,&MainDialog::OnBnClickedBtnclearlog)
    ON_BN_CLICKED(IDC_BTNSetting,&MainDialog::OnBnClickedBtnsetting)
    ON_CBN_SELCHANGE(IDC_CMBBaud,&MainDialog::OnCbnSelchangeCmbbaud)
    ON_CBN_SELCHANGE(IDC_CMBProtocolType,&MainDialog::OnCbnSelchangeCmbprotocoltype)
    ON_CBN_SELCHANGE(IDC_CMBWay,&MainDialog::OnCbnSelchangeCmbway)
END_MESSAGE_MAP()

void MainDialog::OnOK(){}

BOOL MainDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon,TRUE);
    SetIcon(m_hIcon,FALSE);

    (new MyThread([&](){initMainControl(this);},true))->start();

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

//串口线程消息处理函数
LRESULT MainDialog::serialResponse(WPARAM w,LPARAM l){
    static bool continueImage=false;
    switch(w){
    case WM_GET_CON_IMAGE:
    {
        continueImage=true;
    }
    case WM_GET_RAW_IMAGE:
    {
        if(continueImage){
            comm.request(SII(GetRawImage));
            progress->SetPos(30);
            MyLog::user("请放手指");
        } else{
            MainDialogCtrlValidity::Work();
        }
    }break;
    case WM_STP_GET_IMAGE:
    {
        continueImage=false;
        MainDialogCtrlValidity::Work();
        progress->SetPos(0);
    }break;
    case WM_READ_REGISTER:
    {
        progress->SetPos(100);
    }break;
    case WM_APPEND_CONTROLS:
    {
        advancedDebugDialog->append(MyString::ParseInt(conf["id"]),conf["AdvDbg_ImgId"]);
        conf["AdvDbg_ImgId"]=MyString::IntToMyString(MyString::ParseInt(conf["AdvDbg_ImgId"])+1);
    }break;
    case WM_GET_CON_BKI:
    {
        continueImage=true;
    }
    case WM_GET_TEST_IMAGE:
    {
        if(continueImage){
            comm.request(SII(GetTestImage));
            progress->SetPos(30);
        } else{
            MainDialogCtrlValidity::Work();
        }
    }break;
    case WM_STP_GET_BKI:
    {
        continueImage=false;
        MainDialogCtrlValidity::Work();
        progress->SetPos(0);
    }break;
    }
    progress->SetPos(0);
    return 1;
}
