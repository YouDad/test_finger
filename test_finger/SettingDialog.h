#pragma once
#include "stdafx.h"

class SettingDialog: public CDialogEx{
    DECLARE_DYNAMIC(SettingDialog)

public:
    SettingDialog(CWnd* pParent=nullptr);
    virtual ~SettingDialog();

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_SETTING_DIALOG
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
    
public:
    virtual void OnOK();
    afx_msg void OnTcnSelchangeSettingTab(NMHDR *pNMHDR,LRESULT *pResult);
};
