#pragma once

struct Command{
    MyString Name;
    int CmdCode;
    std::vector<uint8_t> DfaultValues;
};

class CommandDialog: public CDialogEx{
    DECLARE_DYNAMIC(CommandDialog)

public:
    CommandDialog(std::vector<struct Command>& v,CWnd* pParent=nullptr);
    virtual ~CommandDialog();
    //消息处理函数
    afx_msg LRESULT response(WPARAM w,LPARAM l);

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_TabsDialog
    };
#endif

protected:
    std::vector<struct Command> commands;
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()

public:
    virtual void OnOK();
    afx_msg void OnClick();
};
extern int isFreeRequest;
