#include "stdafx.h"
#include "test_finger.h"
#include "test_fingerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




CEdit* editLog;
CEdit* editAddress;
CEdit* editPassword;
CEdit* editAddressSet;
CEdit* editPasswordSet;
CEdit* editLightTime;
CEdit* editSensitivity;
CComboBox* cmbWay;
CComboBox* cmbBaud;
CComboBox* cmbBaudSet;
CComboBox* cmbSecurity;
CButton* btnConnect;
CButton* btnReset;
CButton* btnRawImage;
CButton* btnContinueImage;
CButton* btnSetSecurity;
CButton* btnSetCmos;
CButton* btnSetBaud;
CButton* btnSetPassword;
CButton* btnSetAddress;
CButton* btnSaveLog;
CStatic* textDevice;
CStatic* image;


// Ctest_fingerDlg 对话框

Ctest_fingerDlg::Ctest_fingerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctest_fingerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctest_fingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Ctest_fingerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BTNConnect, &Ctest_fingerDlg::OnBnClickedBtnconnect)
	ON_BN_CLICKED(IDC_BTNSaveLog, &Ctest_fingerDlg::OnBnClickedBtnsavelog)
END_MESSAGE_MAP()


// Ctest_fingerDlg 消息处理程序

BOOL Ctest_fingerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	///1.全局控件赋值
	editLog			= (CEdit*) GetDlgItem(IDC_EDITLog);
	editAddress		= (CEdit*) GetDlgItem(IDC_EDITAddress);
	editPassword	= (CEdit*) GetDlgItem(IDC_EDITPassword);
	editAddressSet	= (CEdit*) GetDlgItem(IDC_EDITAddressSet);
	editPasswordSet	= (CEdit*) GetDlgItem(IDC_EDITPasswordSet);
	editLightTime	= (CEdit*) GetDlgItem(IDC_EDITLightTime);
	editSensitivity	= (CEdit*) GetDlgItem(IDC_EDITSensitivity);
	cmbWay			= (CComboBox*) GetDlgItem(IDC_CMBWay);
	cmbBaud			= (CComboBox*) GetDlgItem(IDC_CMBBaud);
	cmbBaudSet		= (CComboBox*) GetDlgItem(IDC_CMBBaudSet);
	cmbSecurity		= (CComboBox*) GetDlgItem(IDC_CMBSecurity);
	btnConnect		= (CButton*) GetDlgItem(IDC_BTNConnect);
	btnReset		= (CButton*) GetDlgItem(IDC_BTNReset);
	btnRawImage		= (CButton*) GetDlgItem(IDC_BTNRawImage);
	btnContinueImage= (CButton*) GetDlgItem(IDC_BTNContinueImage);
	btnSetSecurity	= (CButton*) GetDlgItem(IDC_BTNSetSecurity);
	btnSetCmos		= (CButton*) GetDlgItem(IDC_BTNSetCmos);
	btnSetBaud		= (CButton*) GetDlgItem(IDC_BTNSetBaud);
	btnSetPassword	= (CButton*) GetDlgItem(IDC_BTNSetPassword);
	btnSetAddress	= (CButton*) GetDlgItem(IDC_BTNSetAddress);
	btnSaveLog		= (CButton*) GetDlgItem(IDC_BTNSaveLog);
	textDevice		= (CStatic*) GetDlgItem(IDC_TEXTDevice);
	image			= (CStatic*) GetDlgItem(IDC_IMAGE);

	///2.对组合框的初始化
	updateCommunityWay();
	const WCHAR* baud[]={_T("9600"),_T("115200"),0};//常用波特率
	for(int i=0;baud[i];i++){
		cmbBaud->InsertString(i,baud[i]);
		cmbBaudSet->InsertString(i,baud[i]);
	}
	cmbBaud->SetCurSel(1);
	const WCHAR* security[]={_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),0};//指纹安全级别
	for(int i=0;security[i];i++){
		cmbSecurity->InsertString(i,security[i]);
	}
	cmbSecurity->SetCurSel(2);

	///3.更新 各控件访问权限
	updateControlDisable(actInit);


	///0.测试
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ctest_fingerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ctest_fingerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//支持串口热拔插的设备更改监听函数
#include<Dbt.h>
afx_msg BOOL Ctest_fingerDlg::OnDeviceChange(UINT nEventType, DWORD dwData){
	switch(nEventType){
		case DBT_DEVICEREMOVECOMPLETE://移除设备
		case DBT_DEVICEARRIVAL://添加设备
			updateCommunityWay();
			break;
	}
	return TRUE;
}

//连接下位机按钮的点击事件
void Ctest_fingerDlg::OnBnClickedBtnconnect(){
	
	loadImage(_T("res/preview.bmp"));

	//根据按钮上的文字判断当前连接状态
	WCHAR str[512];
	btnConnect->GetWindowTextW(str,512);
	if(lstrcmp(str,_T("连接下位机"))==0){
		updateControlDisable(actOpeningPort);

		LONG baud=GetDlgItemInt(IDC_CMBBaud);//读取波特率
		cmbWay->GetWindowText(str,512);
		CString way(str);int com;
		swscanf(way,_T("COM%d"),&com);//读取通信方式
		
		way.Format(_T("\\\\.\\COM%d"),com);//得到端口地址
		LONG retCode=ERROR_SUCCESS;

		if(retCode==ERROR_SUCCESS)
			retCode=serial.Open(way,2048,2048,true);

		//其余三参数均采用默认参数
		if(retCode==ERROR_SUCCESS)
			retCode=serial.Setup((CSerial::EBaudrate)baud);

		if(retCode==ERROR_SUCCESS)
			retCode=serial.SetMask(CSerial::EEventRecv);
		if(retCode==ERROR_SUCCESS){
			log("连接COM%d成功",com);
			updateControlDisable(actOpenedPort);
			btnConnect->SetWindowText(_T("断开连接"));
			serial.Purge();
		}else{
			log("连接COM%d失败",com);
			updateControlDisable(actClosedPort);
		}
	}else{
		serial.Close();
		updateControlDisable(actClosedPort);
		log("断开连接成功");
		btnConnect->SetWindowText(_T("连接下位机"));
	}
}


void Ctest_fingerDlg::OnBnClickedBtnsavelog(){
	
}
