#pragma once

class SettingCommonPage: public CDialogEx{
    DECLARE_DYNAMIC(SettingCommonPage)

public:
    SettingCommonPage(CWnd* pParent=nullptr);
    virtual ~SettingCommonPage();

#ifdef AFX_DESIGN_TIME
    enum{ 
        IDD=IDD_SettingCommonPage
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnApply();
    afx_msg void OnBnClickedBtnCancel();
    afx_msg void OnBnClickedBtnQuitAdvDbg();
    afx_msg void OnBnClickedChkSaveConf();
    afx_msg void OnBnClickedChkautocheck();
    afx_msg void OnBnClickedChkautolog();
    afx_msg void OnBnClickedChkrembaud();
    afx_msg void OnBnClickedChkremprotocol();
};
