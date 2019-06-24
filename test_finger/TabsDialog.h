#pragma once

struct Tab{
    CDialogEx* dlg;
    MyString name;
    int templateID;
};

class TabsDialog: public CDialogEx{
    DECLARE_DYNAMIC(TabsDialog)

public:
    TabsDialog(std::vector<Tab>& v,CWnd* pParent=nullptr);
    virtual ~TabsDialog();
    //消息处理函数
    afx_msg LRESULT response(WPARAM w,LPARAM l);

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_TabsDialog
    };
#endif

protected:
    std::vector<Tab> TabPages;
    CTabCtrl* Tabs;
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
    
public:
    virtual void OnOK();
    afx_msg void OnTcnSelChangeTab(NMHDR *pNMHDR,LRESULT *pResult);
};
