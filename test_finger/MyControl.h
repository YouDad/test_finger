#pragma once
#include"stdafx.h"

MyString getText(CWnd* pWnd);
void setText(CWnd* pWnd,MyString str);

int getInt(CWnd* pWnd);
int getHex(CWnd* pWnd);

void initMyControl(MainDialog* Dlg);
void sendMessage(int Message);

extern CEdit* editLog;
extern CEdit* editNow;
extern CEdit* editAddress;
extern CEdit* editPassword;
extern CEdit* editReadRegAddr;
extern CEdit* editReadRegVal;
extern CEdit* editWriteRegAddr;
extern CEdit* editWriteRegVal;
extern CEdit* editFingerId;
extern CComboBox* cmbWay;
extern CComboBox* cmbBaud;
extern CComboBox* cmbProtocolType;
extern CComboBox* cmbLogLevel;
extern CButton* btnConnect;
extern CButton* btnAdvDbg;
extern CButton* btnRawImage;
extern CButton* btnTestImage;
extern CButton* btnContinueImage;
extern CButton* btnContinueBackGroundImage;
extern CButton* btnSaveLog;
extern CButton* btnReadReg;
extern CButton* btnWriteReg;
extern CButton* btnEnroll;
extern CButton* btnMatch;
extern CButton* btnDeviceInfo;
extern CButton* btnViewEnrollIds;
extern CButton* btnCancel;
extern CButton* btnClearLog;
extern CStatic* image;
extern CProgressCtrl* progress;

extern HWND hwnd;

extern AdvancedDebugDialog* advancedDebugDialog;
