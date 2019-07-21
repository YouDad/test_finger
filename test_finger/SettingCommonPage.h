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
    afx_msg void OnBnClickedChkAutoCheck();
    afx_msg void OnBnClickedChkAutoLog();
    afx_msg void OnBnClickedChkRemBaud();
    afx_msg void OnBnClickedChkRemProtocol();
    afx_msg void OnEnChangeEditimgsize();
    afx_msg void OnNMCustomdrawSliderImgSize(NMHDR *pNMHDR,LRESULT *pResult);
    afx_msg void OnBnClickedChkRemAddress();
};
