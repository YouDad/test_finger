#include "stdafx.h"
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
CEdit* editReadRegAddr;
CEdit* editReadRegVal;
CEdit* editWriteRegAddr;
CEdit* editWriteRegVal;
CComboBox* cmbWay;
CComboBox* cmbBaud;
CComboBox* cmbBaudSet;
CComboBox* cmbSecurity;
CComboBox* cmbLogLevel;
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
CButton* btnReadReg;
CButton* btnWriteReg;
CButton* radImgSize1;
CButton* radImgSize2;
CButton* radImgSize3;
CButton* radImgSize4;
CStatic* textDevice;
CStatic* image;
CProgressCtrl* progress;
BYTE packet[65536];
DWORD packetCnt;
BYTE packetData[65536];
DWORD packetDataLen;


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
	ON_MESSAGE(WM_GET_RAW_IMAGE,serialResponse)
	ON_MESSAGE(WM_GET_CON_IMAGE,serialResponse)
	ON_MESSAGE(WM_STP_GET_IMAGE,serialResponse)
	ON_MESSAGE(WM_READ_REGISTER,serialResponse)
	ON_MESSAGE(WM_WRITE_REGISTER,serialResponse)
	ON_WM_QUERYDRAGICON()
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_BTNConnect, &Ctest_fingerDlg::OnBnClickedBtnconnect)
	ON_BN_CLICKED(IDC_BTNSaveLog, &Ctest_fingerDlg::OnBnClickedBtnsavelog)
	ON_BN_CLICKED(IDC_BTNRawImage, &Ctest_fingerDlg::OnBnClickedBtnrawimage)
	ON_BN_CLICKED(IDC_BTNContinueImage, &Ctest_fingerDlg::OnBnClickedBtncontinueimage)
	ON_BN_CLICKED(IDC_BTNdevLog, &Ctest_fingerDlg::OnBnClickedBtndevlog)
	ON_BN_CLICKED(IDC_BTNreadReg, &Ctest_fingerDlg::OnBnClickedBtnreadreg)
	ON_BN_CLICKED(IDC_BTNwriteReg, &Ctest_fingerDlg::OnBnClickedBtnwritereg)
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
	editReadRegAddr	= (CEdit*) GetDlgItem(IDC_EDITreadRegAddr);
	editReadRegVal	= (CEdit*) GetDlgItem(IDC_EDITreadRegVal);
	editWriteRegAddr= (CEdit*) GetDlgItem(IDC_EDITwriteRegAddr);
	editWriteRegVal	= (CEdit*) GetDlgItem(IDC_EDITwriteRegVal);
	cmbWay			= (CComboBox*) GetDlgItem(IDC_CMBWay);
	cmbBaud			= (CComboBox*) GetDlgItem(IDC_CMBBaud);
	cmbBaudSet		= (CComboBox*) GetDlgItem(IDC_CMBBaudSet);
	cmbSecurity		= (CComboBox*) GetDlgItem(IDC_CMBSecurity);
	cmbLogLevel		= (CComboBox*) GetDlgItem(IDC_CMBLogLevel);
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
	btnReadReg		= (CButton*) GetDlgItem(IDC_BTNreadReg);
	btnWriteReg		= (CButton*) GetDlgItem(IDC_BTNwriteReg);
	radImgSize1		= (CButton*) GetDlgItem(IDC_RADimgSize1);
	radImgSize2		= (CButton*) GetDlgItem(IDC_RADimgSize2);
	radImgSize3		= (CButton*) GetDlgItem(IDC_RADimgSize3);
	radImgSize4		= (CButton*) GetDlgItem(IDC_RADimgSize4);
	textDevice		= (CStatic*) GetDlgItem(IDC_TEXTDevice);
	image			= (CStatic*) GetDlgItem(IDC_IMAGE);
	progress		= (CProgressCtrl*) GetDlgItem(IDC_PROGRESS);

	///2.对组合框的初始化
	updateCommunityWay();
	//常用波特率
	const WCHAR* baud[]={_T("9600"),_T("115200"),_T("256000"),0};
	for(int i=0;baud[i];i++){
		cmbBaud->InsertString(i,baud[i]);
		cmbBaudSet->InsertString(i,baud[i]);
	}
	cmbBaud->SetCurSel(1);
	//指纹安全级别
	const WCHAR* security[]={_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),0};
	for(int i=0;security[i];i++){
		cmbSecurity->InsertString(i,security[i]);
	}
	cmbSecurity->SetCurSel(2);
	//日志信息等级
	const WCHAR* logLevel[]={_T("用户"),_T("错误"),_T("警告"),_T("调试"),_T("临时"),0};
	for(int i=0;logLevel[i];i++){
		cmbLogLevel->InsertString(i,logLevel[i]);
	}
	cmbLogLevel->SetCurSel(3);

	///3.更新 各控件访问权限
	updateControlDisable(actInit);

	///4.线程句柄清零
	serialThread=0;

	///5.进度条设置
	progress->SetRange(0,100);
	progress->SetPos(0);

	///6.默认选中160x160
	((CButton*)GetDlgItem(IDC_RADimgSize1))->SetCheck(TRUE);

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
#include "test_fingerDlg.h"
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
			log(LOGU,"连接COM%d成功",com);
			updateControlDisable(actOpenedPort);
			btnConnect->SetWindowText(_T("断开连接"));
			serial.Purge();
		}else{
			log(LOGU,"连接COM%d失败",com);
			updateControlDisable(actClosedPort);
		}
	}else{
		serial.Close();
		updateControlDisable(actClosedPort);
		log(LOGU,"断开连接成功");
		btnConnect->SetWindowText(_T("连接下位机"));
	}
}

//日志保存按钮点击事件
void Ctest_fingerDlg::OnBnClickedBtnsavelog(){
	//获取文件路径名
	LPCTSTR filter=_T("文本文件(*.txt)|*.txt||");
	CFileDialog dlgFileOpen(0,0,0,0,filter,0);
	if(dlgFileOpen.DoModal()==IDOK){
		CString path=dlgFileOpen.GetPathName();
		//get text
		CString logText;
		editLog->GetWindowText(logText);
		//Add .txt ?
		if(path.Find(_T(".txt"),path.GetLength()-4)==-1){
			path=path+_T(".txt");
		}
		//CString->char*
		char filePath[256]={},* text=CString2char(logText);
		int len=WideCharToMultiByte(0,0,path,path.GetLength(),0,0,0,0);
		WideCharToMultiByte(0,0,path,path.GetLength(),filePath,len,0,0);
		//write into file
		FILE* fp=fopen(filePath,"w");
		fprintf(fp,"%s",text);
		fclose(fp);
	}
}

//获取原始图像的线程函数
DWORD WINAPI threadGetRawImage(LPVOID params){
	serial.Purge();
	SendCommand(CMD_GET_RAW_IMAGE,0,0);
	progress->SetPos(30);
	log(LOGD,"Serial Thread:3向下位机发送命令:CMD_GET_RAW_IMAGE");
	log(LOGU,"请放手指");
	serial.Read(packet,sizeof packet,&packetCnt,0,10*1000);
	progress->SetPos(50);
	log(LOGD,"Serial Thread:4接收到数据包,大小为%d",packetCnt);
	log(LOGD,"Serial Thread:5线程向主线程发送消息WM_GET_RAW_IMAGE");
	SendMessage((HWND)params,WM_GET_RAW_IMAGE,WM_GET_RAW_IMAGE,0);
	return 0;
}

//原始图像的点击事件
void Ctest_fingerDlg::OnBnClickedBtnrawimage(){
	updateControlDisable(actGetingImage);
	progress->SetPos(10);
	log(LOGD,"Main Thread:1开始采集原始图像");
	serialThread=CreateThread(0,0,threadGetRawImage,this->m_hWnd,0,0);
	progress->SetPos(20);
    log(LOGD,"Main Thread:2采集图像线程地址:%d",serialThread);
}

//串口线程消息处理函数
LRESULT Ctest_fingerDlg::serialResponse(WPARAM w,LPARAM l){
	static bool continueImage=false;
	switch(w){
		case WM_GET_RAW_IMAGE:{
			progress->SetPos(75);
			log(LOGD,"Main Thread:6消息处理函数收到消息WM_GET_RAW_IMAGE");
			getDataFromPacket();
			
			if(packetDataLen==0){
				log(LOGU,"接收数据超时");
				CloseHandle(serialThread);
				serialThread=0;
			}else{
				CString path=CTime::GetCurrentTime().Format("%Y_%m_%d_%H_%M_%S");
				path=_T("collectedImage/")+path+_T(".bmp");
				if(packetDataLen!=160*160)
					ASF_WARNING(03);
				saveBmp(160,160,packetData,path);
				loadImage((LPTSTR)(LPCTSTR)path);
				progress->SetPos(100);
				log(LOGD,"Main Thread:7加载图片完成");
				log(LOGU,"接收数据成功");
				CloseHandle(serialThread);
				serialThread=0;
			}
			updateControlDisable(actGotImage);
			if(!continueImage){
				break;
			}
		}//if continueImage then can exec next
		//so no break
		case WM_GET_CON_IMAGE:{
			continueImage=true;
			assert(serialThread==0);
			serialThread=CreateThread(0,0,threadGetRawImage,this->m_hWnd,0,0);
		}break;
		case WM_STP_GET_IMAGE:{
			continueImage=false;
			if(serialThread){
				TerminateThread(serialThread,-1);
				CloseHandle(serialThread);
				serialThread=0;
			}
		}break;
		case WM_READ_REGISTER:{
			getDataFromPacket();
			if(packetDataLen==0){
				log(LOGU,"接收数据超时");
				CloseHandle(serialThread);
				serialThread=0;
			}else{
				CString tmp;
				tmp.Format(_T("%X"),packetData[0]);
				editReadRegVal->SetWindowText(tmp);
				log(LOGU,"接收数据成功");
				CloseHandle(serialThread);
				serialThread=0;
			}
			updateControlDisable(actReadedReg);
		}break;
		case WM_WRITE_REGISTER:{
			log(LOGU,"修改寄存器命令发送成功");
			CloseHandle(serialThread);
			serialThread=0;
			updateControlDisable(actWritedReg);
		}break;
	}
	progress->SetPos(0);
	return 1;
}

//连续获取图像的点击事件
void Ctest_fingerDlg::OnBnClickedBtncontinueimage(){
	//根据按钮上的文字判断当前连接状态
	WCHAR str[512];
	btnContinueImage->GetWindowTextW(str,512);
	if(lstrcmp(str,_T("连续获取图像"))==0){
		log(LOGU,"开始连续获取图像");
		updateControlDisable(actGetConImage);
		btnContinueImage->SetWindowText(_T("停止获取图像"));
		SendMessage(WM_GET_CON_IMAGE,WM_GET_CON_IMAGE,0);
	}else{
		log(LOGU,"停止连续获取图像");
		updateControlDisable(actStpGetImage);
		btnContinueImage->SetWindowText(_T("连续获取图像"));
		SendMessage(WM_STP_GET_IMAGE,WM_STP_GET_IMAGE,0);
	}
}


void Ctest_fingerDlg::OnBnClickedBtndevlog(){
	log(LOGU,"V0.9 <时间未知>:完成了串口连接和图片显示,完成了日志功能的建设");
	log(LOGU,"V1.0 <2019年3月16日15:36:11>:完成原始图像和连续取图按钮功能");
	log(LOGU,"V1.1 <2019年3月16日15:54:23>:完成按钮互斥,防止线程冲突,添加开发日志");
	log(LOGU,"V1.2 <2019年3月18日00:57:44>:添加读写寄存器,添加进度条,添加选图像大小");
}

//读寄存器的线程函数
DWORD WINAPI threadReadReg(LPVOID params){
	CString hex;
	int integer;
	editReadRegAddr->GetWindowText(hex);
	sscanf(CString2char(hex),"%X",&integer);
	U8 address=integer;

    serial.Purge();
    SendCommand(CMD_READ_NOTE_BOOK,&address,1);

	serial.Read(packet,sizeof packet,&packetCnt,NULL,1000);

	SendMessage((HWND)params,WM_READ_REGISTER,WM_READ_REGISTER,0);
	return 0;
}

//写寄存器的线程函数
DWORD WINAPI threadWriteReg(LPVOID params){
	CString hex;
	int integer;
	U8 addrVal[2];
	editWriteRegAddr->GetWindowText(hex);
	sscanf(CString2char(hex),"%X",&integer);
	addrVal[0]=integer;
	editWriteRegVal->GetWindowText(hex);
	sscanf(CString2char(hex),"%X",&integer);
	addrVal[1]=integer;

    serial.Purge();
    SendCommand(CMD_WRITE_NOTE_BOOK,addrVal,2);

	SendMessage((HWND)params,WM_WRITE_REGISTER,WM_WRITE_REGISTER,0);
	return 0;
}

void Ctest_fingerDlg::OnBnClickedBtnreadreg(){
	updateControlDisable(actReadingReg);
	progress->SetPos(10);
	log(LOGD,"Main Thread:1开始读寄存器");
	serialThread=CreateThread(0,0,threadReadReg,this->m_hWnd,0,0);
	progress->SetPos(20);
    log(LOGD,"Main Thread:2读寄存器线程地址:%d",serialThread);
}


void Ctest_fingerDlg::OnBnClickedBtnwritereg(){
	updateControlDisable(actWritingReg);
	progress->SetPos(10);
	log(LOGD,"Main Thread:1开始写寄存器");
	serialThread=CreateThread(0,0,threadWriteReg,this->m_hWnd,0,0);
	progress->SetPos(20);
    log(LOGD,"Main Thread:2写寄存器线程地址:%d",serialThread);
}
