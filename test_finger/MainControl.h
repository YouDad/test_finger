#pragma once
#include"stdafx.h"

// 初始化MainDialog的控件指针,以及初始化一些控件设置
void initMainControl(MainDialog* Dlg);

// 向MainDialog的消息处理函数发送消息
void sendMainDialogMessage(int Message);

// 更新通信方式组合框
void updateCommunityWay();
// 尝试自动连接
void autoConnect();
// 自动断开连接
void autoDisconnect();

// 获得通信方式中选择的Com号,USB返回=0,串口返回>0
int getComID();

// 设置进度条进度[0,100]
void setProgress(int percent);
// 设置进度条进度[0.0,100.0]
void setProgress(double percent);

// 对外公开的控件
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
extern AdvancedDebugDialog* advancedDebugDialog;

// font
extern CFont* defaultFont;
