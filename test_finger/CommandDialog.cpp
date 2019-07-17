#include "stdafx.h"

IMPLEMENT_DYNAMIC(CommandDialog,CDialogEx)

// 构造函数
CommandDialog::CommandDialog(std::vector<struct Command>& v,CWnd* pParent)
    : CDialogEx(IDD_CommandDialog,pParent){
    commands=v;
    Create(IDD_CommandDialog,this);
    ShowWindow(SW_SHOW);
}

// 析构函数
CommandDialog::~CommandDialog(){
    //TODO 析构
}

LRESULT CommandDialog::response(WPARAM w,LPARAM l){

    return TRUE;
}

// 初始化事件
BOOL CommandDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();

    // TODO 动态增加控件
    return 0;
}

// 阻止Enter关闭窗口
void CommandDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(CommandDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
END_MESSAGE_MAP()
