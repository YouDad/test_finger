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

HWND hwnd;

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

void initMyControl(Ctest_fingerDlg* Dlg){
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
    cmbChipType=(CComboBox*)Dlg->GetDlgItem(IDC_CMBChipType);
    cmbLogLevel=(CComboBox*)Dlg->GetDlgItem(IDC_CMBLogLevel);
    btnConnect=(CButton*)Dlg->GetDlgItem(IDC_BTNConnect);
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
}

void sendMessage(int Message){
    SendMessage(hwnd,Message,Message,0);
}
