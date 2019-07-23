#include "stdafx.h"

IMPLEMENT_DYNAMIC(TabsDialog,CDialogEx)

// 构造函数
TabsDialog::TabsDialog(std::vector<Tab>& v,CWnd* pParent)
    : CDialogEx(IDD_TabsDialog,pParent){
    TabPages=v;
    Create(IDD_TabsDialog,this);
    ShowWindow(SW_SHOW);
}

// 析构函数
TabsDialog::~TabsDialog(){
    for(auto it=TabPages.begin();it!=TabPages.end();it++){
        DEL_INFO;
        delete it->dlg;
    }
}

LRESULT TabsDialog::response(WPARAM w,LPARAM l){
    for(int i=0;i<TabPages.size();i++){
        ::SendMessage(TabPages[i].dlg->m_hWnd,w,w,l);
    }
    return TRUE;
}

// 初始化事件
BOOL TabsDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    Tabs=(CTabCtrl*)GetDlgItem(IDC_Tabs);

    CRect tabRect;
    Tabs->GetClientRect(tabRect);
    // 实验数据,基本不变
    tabRect.left+=12;
    tabRect.top+=32;
    tabRect.bottom+=9;
    tabRect.right+=8;
    for(int i=0;i<TabPages.size();i++){
        Tabs->InsertItem(i,TabPages[i].name);
        TabPages[i].dlg->Create(TabPages[i].templateID,Tabs);
        TabPages[i].dlg->SetParent(this);
        TabPages[i].dlg->MoveWindow(&tabRect);
        TabPages[i].dlg->ShowWindow(i==0?SW_SHOW:SW_HIDE);
    }
    return 0;
}

// 阻止Enter关闭窗口
void TabsDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(TabsDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
    ON_NOTIFY(TCN_SELCHANGE,IDC_Tabs,&TabsDialog::OnTcnSelChangeTab)
END_MESSAGE_MAP()

void TabsDialog::OnTcnSelChangeTab(NMHDR *pNMHDR,LRESULT *pResult){
    *pResult=0;
    int j=Tabs->GetCurSel();
    for(int i=0;i<TabPages.size();i++){
        TabPages[i].dlg->ShowWindow(i==j?SW_SHOW:SW_HIDE);
    }
}
