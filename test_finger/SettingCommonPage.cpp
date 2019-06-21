// dlg1.cpp: 实现文件
//

#include "stdafx.h"
#include "SettingCommonPage.h"
#include "afxdialogex.h"


// dlg1 对话框

IMPLEMENT_DYNAMIC(SettingCommonPage, CDialogEx)

SettingCommonPage::SettingCommonPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SettingCommonPage, pParent)
{

}

SettingCommonPage::~SettingCommonPage()
{
}

void SettingCommonPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SettingCommonPage, CDialogEx)
END_MESSAGE_MAP()


// dlg1 消息处理程序
