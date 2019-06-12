#pragma once
#include"stdafx.h"

void initMainControl(MainDialog* Dlg);
void sendMainDialogMessage(int Message);

//更新通信方式组合框
void updateCommunityWay();
//自动连接
void autoConnect();
//自动断开连接
void autoDisconnect();

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
extern CButton* btnContinueBGImg;
extern CButton* btnSaveLog;
extern CButton* btnReadReg;
extern CButton* btnWriteReg;
extern CButton* btnEnroll;
extern CButton* btnMatch;
extern CButton* btnDeviceInfo;
extern CButton* btnViewEnrollIds;
extern CButton* btnCancel;
extern CButton* btnClearLog;
extern CButton* btnDeleteTemplate;
extern CStatic* image;
extern CProgressCtrl* progress;

extern HWND hwnd;

extern AdvancedDebugDialog* advancedDebugDialog;
extern std::vector<int>* idle;
extern std::vector<int>* lastIdle;
