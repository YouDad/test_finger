#pragma once
#include "stdafx.h"

class SettingDialog: public CDialogEx{
    DECLARE_DYNAMIC(SettingDialog)

public:
    SettingDialog(CWnd* pParent=nullptr);
    virtual ~SettingDialog();

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_DIALOG1
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
    
public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnCheckUpdate();
    afx_msg void OnBnClickedBtnDownload();
    afx_msg void OnBnClickedChkAutoCheck();
};
