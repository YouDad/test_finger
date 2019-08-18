#pragma once
#include"stdafx.h"

// 主窗口类
class MainDialog: public CDialogEx{
public:
    // 主窗口的构造函数
    MainDialog(CWnd* pParent=NULL);
    enum{
        IDD=IDD_TEST_FINGER_DIALOG
    };
protected:
    HICON m_hIcon;
    // 初始化窗口事件
    virtual BOOL OnInitDialog();
    // 重画事件
    afx_msg void OnPaint();
    // 查询图标事件
    afx_msg HCURSOR OnQueryDragIcon();
    //支持串口热拔插的设备更改监听函数
    afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD dwData);
    //串口线程消息处理函数
    afx_msg LRESULT serialResponse(WPARAM w,LPARAM l);
    DECLARE_MESSAGE_MAP()
public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnConnect();
    afx_msg void OnBnClickedBtnSaveLog();
    afx_msg void OnBnClickedBtnRawImage();
    afx_msg void OnBnClickedBtnContinueImage();
    afx_msg void OnBnClickedBtnDevLog();
    afx_msg void OnBnClickedBtnReadReg();
    afx_msg void OnBnClickedBtnWriteReg();
    afx_msg void OnBnClickedBtnOpenImage();
    afx_msg void OnBnClickedBtnContinueBackgroundImage();
    afx_msg void OnBnClickedBtnOpenBackgroundImage();
    afx_msg void OnBnClickedBtnBackgroundImage();
    afx_msg void OnCbnCloseupCmbLogLevel();
    afx_msg void OnBnClickedBtnAdvDbg();
    afx_msg void OnBnClickedBtn();
    afx_msg void OnBnClickedBtnEnroll();
    afx_msg void OnBnClickedBtnMatch();
    afx_msg void OnBnClickedBtnDeviceInfo();
    afx_msg void OnBnClickedBtnViewEnrollIDs();
    afx_msg void OnBnClickedBtnDeleteTemplate();
    afx_msg void OnBnClickedBtnCancel();
    afx_msg void OnBnClickedBtnClearLog();
    afx_msg void OnBnClickedBtnSetting();
    afx_msg void OnCbnSelChangeCmbBaud();
    afx_msg void OnCbnSelChangeCmbProtocolType();
    afx_msg void OnCbnSelChangeCmbWay();
    afx_msg void OnCbnSelchangeCmbloglevel();
    afx_msg void OnBnClickedBtnTest();
    afx_msg void OnEnChangeEditAddress();
    afx_msg void OnBnClickedBtnSearch();
    afx_msg void OnBnClickedChkLED();
};
