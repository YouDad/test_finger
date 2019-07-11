#pragma once

enum CommandCtrlType{
    CommandCtrlType_int8_t,
    CommandCtrlType_int16_t,
    CommandCtrlType_int32_t,
    CommandCtrlType_hex8_t,
    CommandCtrlType_hex16_t,
    CommandCtrlType_hex32_t,
    CommandCtrlType_file_t,
};

struct Command{
    MyString Name;
    int CmdCode;
    std::vector<std::pair<MyString,CommandCtrlType>> Type;
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
};
