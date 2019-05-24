#pragma once
#include "stdafx.h"

CEdit* editLog;
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
CButton* btnContinueBackGroundImage;
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
    btnContinueBackGroundImage=(CButton*)Dlg->GetDlgItem(IDC_BTNContinueBackGroundImage);
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
    MyString chipType[]={"GD32F30","ASFComm","Syno"};
    for(int i=0;i<3;i++){
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
}

void sendMessage(int Message){
    SendMessage(hwnd,Message,Message,0);
}
