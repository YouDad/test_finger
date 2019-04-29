#pragma once
#include"stdafx.h"

MyString getText(CWnd* pWnd);
void setText(CWnd* pWnd,MyString str);

int getInt(CWnd* pWnd);
int getHex(CWnd* pWnd);

void initMyControl(Ctest_fingerDlg* Dlg);
void sendMessage(int Message);

extern CEdit* editLog;
extern CEdit* editAddress;
extern CEdit* editPassword;
extern CEdit* editAddressSet;
extern CEdit* editPasswordSet;
extern CEdit* editLightTime;
extern CEdit* editSensitivity;
extern CEdit* editReadRegAddr;
extern CEdit* editReadRegVal;
extern CEdit* editWriteRegAddr;
extern CEdit* editWriteRegVal;
extern CComboBox* cmbWay;
extern CComboBox* cmbBaud;
extern CComboBox* cmbBaudSet;
extern CComboBox* cmbChipType;
extern CComboBox* cmbLogLevel;
extern CButton* btnConnect;
extern CButton* btnRawImage;
extern CButton* btnTestImage;
extern CButton* btnContinueImage;
extern CButton* btnContinueBackGroundImage;
extern CButton* btnSetCmos;
extern CButton* btnSetBaud;
extern CButton* btnSetPassword;
extern CButton* btnSetAddress;
extern CButton* btnSaveLog;
extern CButton* btnReadReg;
extern CButton* btnWriteReg;
extern CButton* chkTimeoutContinue;
extern CStatic* textDevice;
extern CStatic* image;
extern CProgressCtrl* progress;

extern HWND hwnd;
