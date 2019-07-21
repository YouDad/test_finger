#include "stdafx.h"

IMPLEMENT_DYNAMIC(CommandListDialog,CDialogEx)

// 构造函数
CommandListDialog::CommandListDialog(CWnd* pParent): CDialogEx(IDD_CommandListDialog,pParent){}

// 析构函数
CommandListDialog::~CommandListDialog(){}

LRESULT CommandListDialog::response(WPARAM w,LPARAM l){

    return TRUE;
}

CListCtrl* listCtrl;
CButton* btnAddPlugin;
CButton* btnRemovePlugin;
CButton* btnOpenPluginFolder;
CButton* btnEditPlugin;

// 初始化事件
BOOL CommandListDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();

    btnAddPlugin=(CButton*)GetDlgItem(IDC_BTNAddPlugin);
    btnRemovePlugin=(CButton*)GetDlgItem(IDC_BTNRemovePlugin);
    btnOpenPluginFolder=(CButton*)GetDlgItem(IDC_BTNOpenPluginFolder);
    btnEditPlugin=(CButton*)GetDlgItem(IDC_BTNEditPlugin);

    listCtrl=(CListCtrl*)GetDlgItem(IDC_LSTPlugin);
    listCtrl->ModifyStyle(LVS_LIST,LVS_REPORT);
    listCtrl->SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
    listCtrl->InsertColumn(0,L"名字",LVCFMT_LEFT,50);
    listCtrl->InsertColumn(1,L"有效性",LVCFMT_LEFT,50);
    listCtrl->InsertColumn(2,L"路径",LVCFMT_LEFT,250-50-50);

    OnBnClickedBtnFlush();

    return 0;
}

// 阻止Enter关闭窗口
void CommandListDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(CommandListDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
    ON_BN_CLICKED(IDC_BTNAddPlugin,&CommandListDialog::OnBnClickedBtnAddPlugin)
    ON_BN_CLICKED(IDC_BTNRemovePlugin,&CommandListDialog::OnBnClickedBtnRemovePlugin)
    ON_BN_CLICKED(IDC_BTNSaveDefaultPlugin,&CommandListDialog::OnBnClickedBtnSaveDefaultPlugin)
    ON_BN_CLICKED(IDC_BTNOpenPluginFolder,&CommandListDialog::OnBnClickedBtnOpenPluginFolder)
    ON_BN_CLICKED(IDC_BTNEditPlugin,&CommandListDialog::OnBnClickedBtnEditPlugin)
    ON_BN_CLICKED(IDC_BTNFlush,&CommandListDialog::OnBnClickedBtnFlush)
END_MESSAGE_MAP()

void CommandListDialog::OnBnClickedBtnAddPlugin(){
    MyString filepath;
    if(MyFile::OpenFileDialog("ini",this,filepath)){
        int CustomCnt=MyString::ParseInt(conf["CustomCnt"]);
        conf[MyString::Format("Custom%d",CustomCnt)]=filepath;
        conf["CustomCnt"]=MyString::IntToMyString(CustomCnt+1);
        OnBnClickedBtnFlush();
    }
}


void CommandListDialog::OnBnClickedBtnRemovePlugin(){
    int position=listCtrl->GetSelectionMark();
    listCtrl->DeleteItem(position);
    int CustomCnt=MyString::ParseInt(conf["CustomCnt"]);
    for(int i=position;i<CustomCnt-1;i++){
        conf[MyString::Format("Custom%d",i)]=conf[MyString::Format("Custom%d",i+1)];
    }
    conf[MyString::Format("Custom%d",CustomCnt-1)]="";
    conf["CustomCnt"]=MyString::IntToMyString(CustomCnt-1);
}


void CommandListDialog::OnBnClickedBtnSaveDefaultPlugin(){
    MyString filepath;
    if(MyFile::SaveAsDialog("DefaultPlugin1","ini",this,filepath)){
        MyFile::SaveDefaultPlugin1(filepath);
    }
    if(MyFile::SaveAsDialog("DefaultPlugin2","ini",this,filepath)){
        MyFile::SaveDefaultPlugin2(filepath);
    }
}


void CommandListDialog::OnBnClickedBtnOpenPluginFolder(){
    int position=listCtrl->GetSelectionMark();
    std::string path=conf[MyString::Format("Custom%d",position)];
    int end=path.rfind('\\');
    MyString PATH=path.substr(0,end);
    ShellExecuteA(NULL,"explore",PATH,NULL,NULL,SW_NORMAL);
}


void CommandListDialog::OnBnClickedBtnEditPlugin(){

}


void CommandListDialog::OnBnClickedBtnFlush(){
    btnAddPlugin->EnableWindow(conf["SaveConf"]==Stringify(true));
    btnRemovePlugin->EnableWindow(conf["SaveConf"]==Stringify(true));
    btnOpenPluginFolder->EnableWindow(conf["SaveConf"]==Stringify(true));
    btnEditPlugin->EnableWindow(conf["SaveConf"]==Stringify(true));
    listCtrl->DeleteAllItems();
    if(conf["SaveConf"]==Stringify(true)){
        int CustomCnt=MyString::ParseInt(conf["CustomCnt"]);
        for(int i=0;i<CustomCnt;i++){
            std::string key=MyString::Format("Custom%d",i);
            MyString TabName;
            std::vector<struct Command> v;
            bool have=MyFile::ReadCommands(conf[key],TabName,v);
            int pos=listCtrl->InsertItem(i,TabName);
            listCtrl->SetItemText(pos,1,have?L"有效":L"无效");
            listCtrl->SetItemText(pos,2,conf[key]);
        }
    }
}
