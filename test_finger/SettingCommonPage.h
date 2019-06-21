#pragma once


// dlg1 对话框

class SettingCommonPage : public CDialogEx
{
	DECLARE_DYNAMIC(SettingCommonPage)

public:
	SettingCommonPage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SettingCommonPage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
