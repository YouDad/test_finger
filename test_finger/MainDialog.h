#pragma once
#include"stdafx.h"

class MainDialog: public CDialogEx{
public:
    MainDialog(CWnd* pParent=NULL);
    enum{
        IDD=IDD_TEST_FINGER_DIALOG
    };
protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD dwData);
    afx_msg LRESULT serialResponse(WPARAM w,LPARAM l);
    DECLARE_MESSAGE_MAP()
public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnconnect();
    afx_msg void OnBnClickedBtnsavelog();
    afx_msg void OnBnClickedBtnrawimage();
    afx_msg void OnBnClickedBtncontinueimage();
    afx_msg void OnBnClickedBtndevlog();
    afx_msg void OnBnClickedBtnreadreg();
    afx_msg void OnBnClickedBtnwritereg();
    afx_msg void OnBnClickedBtnsetcmos();
    afx_msg void OnBnClickedBtnsetbaud();
    afx_msg void OnBnClickedBtnsetpassword();
    afx_msg void OnBnClickedBtnsetaddress();
    afx_msg void OnBnClickedBtnopenimage();
    afx_msg void OnBnClickedBtncontinuebackgroundimage();
    afx_msg void OnBnClickedBtnopenbackgroundimage();
    afx_msg void OnBnClickedBtnbackgroundimage();
    afx_msg void OnCbnCloseupCmbloglevel();
    afx_msg void OnBnClickedBtnadvdbg();
};
