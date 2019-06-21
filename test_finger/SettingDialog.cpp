#include "stdafx.h"
#include "SettingCommonPage.h"
#include "SettingUpdatePage.h"

IMPLEMENT_DYNAMIC(SettingDialog,CDialogEx)

// 构造函数
SettingDialog::SettingDialog(CWnd* pParent): CDialogEx(IDD_SETTING_DIALOG,pParent){}

// 析构函数
SettingDialog::~SettingDialog(){}

// 控件指针群
CTabCtrl* Tabs;
SettingCommonPage settingCommonPage;
SettingUpdatePage settingUpdatePage;
CDialogEx* TabPages[]={&settingCommonPage,&settingUpdatePage};
const int TabPageNumber=sizeof TabPages/sizeof TabPages[0];

// 初始化事件
BOOL SettingDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    Tabs=(CTabCtrl*)GetDlgItem(IDC_SettingTab);

    MyString TabName[]={"常规","更新"};
    for(int i=0;i<sizeof TabName/sizeof MyString;i++){
        Tabs->InsertItem(i,TabName[i]);
    }
    TabPages[0]->Create(IDD_SettingCommonPage,Tabs);
    TabPages[1]->Create(IDD_SettingUpdatePage,Tabs);
    CRect tabRect;
    Tabs->GetClientRect(tabRect);
    tabRect.left+=12;
    tabRect.top+=32;
    tabRect.bottom+=9;
    tabRect.right+=8;
    for(int i=0;i<TabPageNumber;i++){
        TabPages[i]->SetParent(this);
        TabPages[i]->MoveWindow(&tabRect);
        TabPages[i]->ShowWindow(i==0?SW_SHOW:SW_HIDE);
    }
    return 0;
}

// 阻止Enter关闭窗口
void SettingDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(SettingDialog,CDialogEx)
    ON_NOTIFY(TCN_SELCHANGE,IDC_SettingTab,&SettingDialog::OnTcnSelchangeSettingTab)
END_MESSAGE_MAP()

void SettingDialog::OnTcnSelchangeSettingTab(NMHDR *pNMHDR,LRESULT *pResult){
    *pResult=0;
    int j=Tabs->GetCurSel();
    for(int i=0;i<TabPageNumber;i++){
        TabPages[i]->ShowWindow(i==j?SW_SHOW:SW_HIDE);
    }
}
