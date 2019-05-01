#pragma once
#include "stdafx.h"

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
CComboBox* cmbProtocolType;
CComboBox* cmbLogLevel;
CButton* btnConnect;
CButton* btnAdvDbg;
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

HWND hwnd;

AdvancedDebugDialog* advancedDebugDialog;

MyString getText(CWnd * pWnd){
    static CString ret;
    pWnd->GetWindowText(ret);
    return ret;
}

void setText(CWnd * pWnd,MyString str){
    pWnd->SetWindowText(str);
}

int getInt(CWnd * pWnd){
    int ret;
    sscanf(getText(pWnd),"%d",&ret);
    return ret;
}

int getHex(CWnd * pWnd){
    int ret;
    sscanf(getText(pWnd),"%x",&ret);
    return ret;
}

void initMyControl(MainDialog* Dlg){
    editLog=(CEdit*)Dlg->GetDlgItem(IDC_EDITLog);
    editAddress=(CEdit*)Dlg->GetDlgItem(IDC_EDITAddress);
    editPassword=(CEdit*)Dlg->GetDlgItem(IDC_EDITPassword);
    editAddressSet=(CEdit*)Dlg->GetDlgItem(IDC_EDITAddressSet);
    editPasswordSet=(CEdit*)Dlg->GetDlgItem(IDC_EDITPasswordSet);
    editLightTime=(CEdit*)Dlg->GetDlgItem(IDC_EDITLightTime);
    editSensitivity=(CEdit*)Dlg->GetDlgItem(IDC_EDITSensitivity);
    editReadRegAddr=(CEdit*)Dlg->GetDlgItem(IDC_EDITreadRegAddr);
    editReadRegVal=(CEdit*)Dlg->GetDlgItem(IDC_EDITreadRegVal);
    editWriteRegAddr=(CEdit*)Dlg->GetDlgItem(IDC_EDITwriteRegAddr);
    editWriteRegVal=(CEdit*)Dlg->GetDlgItem(IDC_EDITwriteRegVal);
    cmbWay=(CComboBox*)Dlg->GetDlgItem(IDC_CMBWay);
    cmbBaud=(CComboBox*)Dlg->GetDlgItem(IDC_CMBBaud);
    cmbBaudSet=(CComboBox*)Dlg->GetDlgItem(IDC_CMBBaudSet);
    cmbProtocolType=(CComboBox*)Dlg->GetDlgItem(IDC_CMBProtocolType);
    cmbLogLevel=(CComboBox*)Dlg->GetDlgItem(IDC_CMBLogLevel);
    btnConnect=(CButton*)Dlg->GetDlgItem(IDC_BTNConnect);
    btnAdvDbg=(CButton*)Dlg->GetDlgItem(IDC_BTNAdvDbg);
    btnRawImage=(CButton*)Dlg->GetDlgItem(IDC_BTNRawImage);
    btnTestImage=(CButton*)Dlg->GetDlgItem(IDC_BTNTestImage);
    btnContinueImage=(CButton*)Dlg->GetDlgItem(IDC_BTNContinueImage);
    btnContinueBackGroundImage=(CButton*)Dlg->GetDlgItem(IDC_BTNContinueBackGroundImage);
    btnSetCmos=(CButton*)Dlg->GetDlgItem(IDC_BTNSetCmos);
    btnSetBaud=(CButton*)Dlg->GetDlgItem(IDC_BTNSetBaud);
    btnSetPassword=(CButton*)Dlg->GetDlgItem(IDC_BTNSetPassword);
    btnSetAddress=(CButton*)Dlg->GetDlgItem(IDC_BTNSetAddress);
    btnSaveLog=(CButton*)Dlg->GetDlgItem(IDC_BTNSaveLog);
    btnReadReg=(CButton*)Dlg->GetDlgItem(IDC_BTNreadReg);
    btnWriteReg=(CButton*)Dlg->GetDlgItem(IDC_BTNwriteReg);
    chkTimeoutContinue=(CButton*)Dlg->GetDlgItem(IDC_CHKTimeoutContinue);
    textDevice=(CStatic*)Dlg->GetDlgItem(IDC_TEXTDevice);
    image=(CStatic*)Dlg->GetDlgItem(IDC_IMAGE);
    progress=(CProgressCtrl*)Dlg->GetDlgItem(IDC_PROGRESS);

    editLog->SetLimitText(-1);
    hwnd=Dlg->m_hWnd;

    if($::conf["AdvDbg"]=="true"){
        btnAdvDbg->ShowWindow(SW_SHOW);
    }
    advancedDebugDialog=0;

    ////通讯方式
    //const WCHAR* way[]={0};
    //for(int i=0;way[i];i++){
    //    cmbWay->InsertString(i,way[i]);
    //}

    //常用波特率
    MyString baud[]={"9600","19200","57600","115200","230400","460800","921600"};
    for(int i=0;i<7;i++){
        cmbBaud->InsertString(i,baud[i]);
        cmbBaudSet->InsertString(i,baud[i]);
    }
    cmbBaud->SetCurSel(3);

    //芯片类型
    const WCHAR* chipType[]={_T("GD32F30"),_T("ASFComm"),0};
    for(int i=0;chipType[i];i++){
        cmbProtocolType->InsertString(i,chipType[i]);
    }
    cmbProtocolType->SetCurSel(1);

    //日志信息等级
    MyString logLevel[]={"用户","错误","警告","调试","临时"};
    for(int i=0;i<5;i++){
        cmbLogLevel->InsertString(i,logLevel[i]);
    }
    cmbLogLevel->SetCurSel(3);

    //进度条设置
    progress->SetRange(0,100);
    progress->SetPos(0);
}

void sendMessage(int Message){
    SendMessage(hwnd,Message,Message,0);
}
