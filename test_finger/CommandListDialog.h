#pragma once

class CommandListDialog: public CDialogEx{
    DECLARE_DYNAMIC(CommandListDialog)

public:
    CommandListDialog(CWnd* pParent=nullptr);
    virtual ~CommandListDialog();
    //消息处理函数
    afx_msg LRESULT response(WPARAM w,LPARAM l);

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_CommandListDialog
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnAddPlugin();
    afx_msg void OnBnClickedBtnRemovePlugin();
    afx_msg void OnBnClickedBtnSaveDefaultPlugin();
    afx_msg void OnBnClickedBtnOpenPluginFolder();
    afx_msg void OnBnClickedBtnEditPlugin();
    afx_msg void OnBnClickedBtnFlush();
};
