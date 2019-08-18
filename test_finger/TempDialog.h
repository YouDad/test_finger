#pragma once

class TempDialog: public CDialogEx{
    DECLARE_DYNAMIC(TempDialog)

public:
    TempDialog(CWnd* pParent=nullptr);
    virtual ~TempDialog();
    //消息处理函数
    afx_msg LRESULT response(WPARAM w,LPARAM l);

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_TempDialog
    };
#endif

protected:
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnOK();
    afx_msg void OnBnClickedBtnDownImage();
    afx_msg void OnBnClickedBtnWriteNotepad();
    afx_msg void OnBnClickedBtnSelectImage();
    afx_msg void OnBnClickedBtnSelectNotepad();
    afx_msg void OnBnClickedBtnDownChar();
};
extern int isFreeRequest;
