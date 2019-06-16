#pragma once

class AdvancedDebugDialog: public CDialogEx{
    DECLARE_DYNAMIC(AdvancedDebugDialog)

public:
    AdvancedDebugDialog(CWnd* pParent=nullptr);
    virtual ~AdvancedDebugDialog();

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_ADVANCED_DEBUG_DIALOG
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnOK();
    void append(int id,MyString name);
    afx_msg void OnBnClickedBtnClearAll();
    afx_msg BOOL PreTranslateMessage(MSG* pMsg);
};
