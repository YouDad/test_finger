// USBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USB.h"
#include "USBDlg.h"
#include "protocol.h"
#include <Shlwapi.h>
#include <stdio.h>
#include <windows.h>
#include <dbt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "command.h"
#pragma comment(lib, "shlwapi.lib")


//#define BaudRate B57600 //波特率
#define DataBits BIT_8 //数据位
#define Parity P_NONE //效验位
#define StopBits STOP_1 //停止位

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

int currentPort=0;
CUIntArray ports,portse,portsu;



///////////////////////////串口中断回调函数//////////////////////////////////
VOID CALLBACK CntIrq(int port){
    if(::AfxGetMainWnd()){
        if(::AfxGetMainWnd()->m_hWnd){
            ::PostMessage(::AfxGetMainWnd()->m_hWnd,WM_PCOMM,0,0);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CUSBDlg dialog

CUSBDlg::CUSBDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CUSBDlg::IDD,pParent){
    //{{AFX_DATA_INIT(CUSBDlg)
    m_strResultMsg=_T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    //	CString str="1\r\n2\r\n";
    //	m_strResultMsg = _T("1\r\n2\r\n");
    //	GetDlgItem(IDC_INFO)->SetWindowText(str);

}

void CUSBDlg::DoDataExchange(CDataExchange* pDX){
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CUSBDlg)
    DDX_Control(pDX,IDC_BtnEnroll,m_ctrlBtnEnroll);
    DDX_Control(pDX,IDC_ListRecord,m_ctrlListRecord);
    DDX_Control(pDX,IDC_BUTTON2,m_ctrlBtnBlackWhite);
    DDX_Control(pDX,IDC_BtnContinuousGetImage,m_ctrlBtnContinuGetImage);
    DDX_Control(pDX,IDC_BtnPortOpen,m_ctrlPortOpen);
    DDX_Control(pDX,IDC_AdjustImage,m_ctrlBtnAdjustImage);
    DDX_Control(pDX,IDC_BtnWrReg,m_ctrlBtnWrReg);
    DDX_Control(pDX,IDC_BtnRdReg,m_ctrlBtnRdReg);
    DDX_Control(pDX,IDC_BtnIdle,m_ctrlBtnGoIdle);
    DDX_Control(pDX,IDC_BtnGoSleep,m_ctrlBtnGoSleep);
    DDX_Control(pDX,IDC_BtnGetRawImage,m_ctrlBtnGetRawImage);
    DDX_Text(pDX,IDC_INFO,m_strResultMsg);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUSBDlg,CDialog)
    //{{AFX_MSG_MAP(CUSBDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BtnPortOpen,OnPortopen)
    ON_BN_CLICKED(IDC_BtnGetRawImage,OnBtnGetRawImage)
    ON_BN_CLICKED(IDC_BtnRdReg,OnBtnRdReg)
    ON_BN_CLICKED(IDC_BtnWrReg,OnBtnWrReg)
    ON_BN_CLICKED(IDC_BtnGoSleep,OnBtnGoSleep)
    ON_BN_CLICKED(IDC_BtnIdle,OnBtnIdle)
    ON_BN_CLICKED(IDC_AdjustImage,OnAdjustImage)
    ON_BN_CLICKED(IDC_BtnContinuousGetImage,OnBtnContinuousGetImage)
    ON_BN_CLICKED(IDC_BUTTON2,OnBlackWhite)
    ON_BN_CLICKED(IDC_BtnRecodeDel,OnBtnRecodeDel)
    ON_NOTIFY(NM_RCLICK,IDC_ListRecord,OnClickListRecord)
    //}}AFX_MSG_MAP
//	ON_MESSAGE(WM_PCOMM,OnPcomm) //这里是消息影射
	ON_WM_DEVICECHANGE()
	ON_MESSAGE(WM_UPDATEDATA,OnUpdateData)
END_MESSAGE_MAP()
////////////////////////////////////////////



///*communication message listen
//	true代表串口监听函数来处理
//	false代表串口监听函数不处理
//	只要在SendCommand之前设置
//	此标志位即可达到选择是否让监听函数处理的目的
//	监听函数为:
//	UINT AFX_CDECL OnCommDataRecv(LPVOID hwnd);
//*/
bool commMsgListen=true;
//UINT AFX_CDECL OnCommDataRecv(LPVOID hwnd){
//	while(true){
//		if(g_Serial.IsOpen()&&g_Serial.WaitEvent()){
//			static char data[65536]={},edit[65536]={};
//			DWORD readCnt;
//			if(commMsgListen){
//				LONG res=g_Serial.Read(data,6000,&readCnt,NULL,5*1000);
//				if(ERROR_SUCCESS==res){
//					GetWindowText((HWND)hwnd,edit,65536);
//					int slen=strlen(edit);
//					edit[slen]='\r';
//					data[readCnt]=0;
//					strcpy(edit+slen+1,data);
//					SetWindowText((HWND)hwnd,edit);
//				}
//			}
//		}
//	}
//	return 0;
//}


///////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CUSBDlg message handlers

BOOL CUSBDlg::OnInitDialog(){
    CDialog::OnInitDialog();

    CMenu* pSysMenu=GetSystemMenu(FALSE);

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon,TRUE);// Set big icon
    SetIcon(m_hIcon,FALSE);// Set small icon

    // TODO: Add extra initialization here
    TraversalCom();

    //串口波特率设置
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(0,"请选择");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(1,"1200");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(2,"1800");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(3,"2400");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(4,"4800");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(5,"7200");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(6,"9600");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(7,"19200");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(8,"38400");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(9,"57600");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->InsertString(10,"115200");
    ((CComboBox *)GetDlgItem(IDC_CBComBaudRate))->SetCurSel(10);
    SetDlgItemText(IDC_EdRdAddr,"0");
    SetDlgItemText(IDC_EdRdValue,"0");
    SetDlgItemText(IDC_EdWrAddr,"0");
    SetDlgItemText(IDC_EdWrValue,"0");
    IdListInit();

	//AfxBeginThread(OnCommDataRecv,(LPVOID)GetDlgItem(IDC_EditCommLog));

    return TRUE;  // return TRUE  unless you set the focus to a control
}
void CUSBDlg::IdListInit(){
    m_ctrlListRecord.InsertColumn(0,_T("ID"),LVCFMT_CENTER,50);
    m_ctrlListRecord.InsertColumn(1,_T("Role"),LVCFMT_CENTER,100);
    m_ctrlListRecord.InsertColumn(2,_T("Quality"),LVCFMT_CENTER,100);
}
void CUSBDlg::OnSysCommand(UINT nID,LPARAM lParam){
    CDialog::OnSysCommand(nID,lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUSBDlg::OnPaint(){
    if(IsIconic()){
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND,(WPARAM)dc.GetSafeHdc(),0);

        // Center icon in client rectangle
        int cxIcon=GetSystemMetrics(SM_CXICON);
        int cyIcon=GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x=(rect.Width()-cxIcon+1)/2;
        int y=(rect.Height()-cyIcon+1)/2;

        // Draw the icon
        dc.DrawIcon(x,y,m_hIcon);
    } else{
        CDialog::OnPaint();
        //
        ////CDrawDoc *pDoc=GetDocument();
        //// pDoc->UpdateAllViews(this);
        //CRect rect;
        //CWnd* pWnd = GetDlgItem(IDC_IMAGE3 );
        //pWnd->GetClientRect(&rect);//得到控件客户端区域坐标
        //pWnd->ClientToScreen(rect);//将区域坐标由 控件客户区转成对话框区
        ////pWnd->GetWindowRect(&rect); //直接得到控件的对话框区坐标
        //this->ScreenToClient(rect); //将区域坐标由 对话框区转成对话框客户区坐标
        ////InvalidateRect(rect);
        //// UpdateWindow();
        //RedrawWindow(rect);
        ////复制代码
        //这里面有几个需要注意的点：
        //1.可以通过GetWindowRect直接获取控件相对于对话框区坐标。
        //2. 注意对话框区和对话框客户区的区别。(参考链接2)
        //3.在这里，InvalidateRect(rect);和RedrawWindow(rect)效果一样，这是由于消息队列消息很少，所以执行很快，但需要注意：
        //InvalidateRect()不能实现立即更新 ；
        //InvalidateRect()与 UpdateWindow()结合后可以实现立即更新；
        //两者结合的效果相当于RedrawWindow(rect)。

        /////////////////////////////////////////////////////////////////////////////////////
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUSBDlg::OnQueryDragIcon(){
    return (HCURSOR)m_hIcon;
}


void CUSBDlg::saveimage(int width,int height,unsigned char *image){
    //    time_t ltime;
    //    struct tm* pnow;
    HANDLE hFile;
    BYTE *pData=NULL;
    int i,j;//, n
    BITMAPFILEHEADER bmfHdr;
    BITMAPINFOHEADER bi;
    DWORD dwBytesWrite;
    uint8_t *image2;
    image2=image;
    //	int i,j;
    //	int width, height;
    //	BYTE qrData[MAX_MODULESIZE][MAX_MODULESIZE];
    //图像大小
    //width = height = 30;

    if(width<=0||height<=0)
        return;

    // allocate memory for data area
    byte Datalength=width;//(width*3+3)/4*4;
    pData=(BYTE*)malloc(Datalength* height);
    if(pData==NULL)
        return;

    // generate random data
    //为每一个像素赋值
  //  srand((unsigned)time(NULL));
    for(i=0; i<(int)(height); i++){
        for(j=0; j<(int)(width); j++){
            pData[i*Datalength+j]=*(image++);

        }
    }

    //initialize the structures

    RtlZeroMemory(&bmfHdr,sizeof(bmfHdr));
    RtlZeroMemory(&bi,sizeof(bi));

    // fill the necessary struct fields of the BITMAPFILEHEADER
//     bmfHdr.bfType = 0x4d42; // "BM"
//     bmfHdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +256*4+ width * height;
//     bmfHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
// 
    // fill the necessary struct fields of the BITMAPINFOHEADER

//    bi.biSize = sizeof(bi);
//    bi.biBitCount = 24;
//    bi.biHeight = -height;
//    bi.biWidth = width;
//    bi.biCompression = BI_RGB;    
//     bi.biPlanes = 1;


    BITMAPINFOHEADER bmpInfo;
    bmpInfo.biSize=sizeof(BITMAPINFOHEADER);
    bmpInfo.biWidth=width;
    bmpInfo.biHeight=-height;
    bmpInfo.biPlanes=1;
    bmpInfo.biBitCount=8;
    bmpInfo.biCompression=BI_RGB;
    bmpInfo.biSizeImage=0;
    bmpInfo.biXPelsPerMeter=0;
    bmpInfo.biYPelsPerMeter=0;
    bmpInfo.biClrUsed=0;
    bmpInfo.biClrImportant=0;
    RGBQUAD  bmfColorQuad[256];
    for(i=0;i<256;i++){
        bmfColorQuad[i].rgbBlue=i;
        bmfColorQuad[i].rgbGreen=i;
        bmfColorQuad[i].rgbRed=i;
        bmfColorQuad[i].rgbReserved=0;
    }
    BITMAPFILEHEADER bmpFileInfo;
    bmpFileInfo.bfType=0x4d42;//"BM"
    bmpFileInfo.bfSize=0;//sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
    bmpFileInfo.bfReserved1=0;
    bmpFileInfo.bfReserved2=0;
    bmpFileInfo.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+1024;


    CTime time=CTime::GetCurrentTime();
    //    /* according current date and time to generate file name, saved in c:\ */
    //
    CString  dataStr=time.Format("%Y_%m_%d_%H_%M_%S");
    dataStr="collectedImage/"+dataStr+".bmp";

	CreateDirectory("collectedImage",0);
    hFile=CreateFile(dataStr,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(hFile==INVALID_HANDLE_VALUE){
        free(pData);
        return;
    }

    // write the BITMAPFILEHEADER

    WriteFile(hFile,&bmpFileInfo,sizeof(BITMAPFILEHEADER),&dwBytesWrite,NULL);
    // write the BITMAPINFOHEADER
    WriteFile(hFile,&bmpInfo,sizeof(BITMAPINFOHEADER),&dwBytesWrite,NULL);//BITMAPINFOHEADER
    // write the bmp data
  //调色板
    WriteFile(hFile,&bmfColorQuad,256*4,&dwBytesWrite,NULL);

    WriteFile(hFile,pData,Datalength * height,&dwBytesWrite,NULL);

    // release resources

    CloseHandle(hFile);
    //////////////////////////////////////////
    unsigned char arry0[160][160];
    CBitmap m_bmp;
    int iWidth=160;
    int iHeight=160;
    //	int imagept=0;
        //随机生成颜色(int)time(0)
        //srand(0xaa55cc);
    image=image2;
    for(i=0; i<iHeight; i++){
        for(j=0; j<iWidth; j++){
            arry0[i][j]=*(image++);// rand()%256;
        }
    }
    //计时开始
//    clock_t  clockBegin, clockEnd;
//     clockBegin = clock();
    int iBytesWidth;
    iBytesWidth=(iWidth*24+31)/32*4; //一个像素占3个字节，24位。然后转化为4字节整数倍

    BYTE *Image=new BYTE[iBytesWidth * iHeight*3];

    for(i=0; i<iHeight; i++){
        for(int j=0; j<iWidth; j++){
            *(Image+i*iBytesWidth+j*3)=arry0[iHeight-i-1][j];//image的左下角对应数组的左上角，原点坐标不一样
            *(Image+1+i*iBytesWidth+j*3)=arry0[iHeight-i-1][j];
            *(Image+2+i*iBytesWidth+j*3)=arry0[iHeight-i-1][j];
        }
    }

    BITMAPINFO bmi;
    ZeroMemory(&bmi,sizeof(BITMAPINFO));
    bmi.bmiHeader.biSizeImage=(iWidth*24+31)/32*4*iHeight;
    bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth=iWidth;
    bmi.bmiHeader.biHeight=iHeight;
    bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biBitCount=24;
    bmi.bmiHeader.biCompression=BI_RGB;

    CDC * dc;
    dc=GetDlgItem(IDC_IMAGE3)->GetDC();  // 获取picture控件DC


    CRect rc;
    CWnd *pWnd=GetDlgItem(IDC_IMAGE3);//参数为控件ID。
    pWnd->GetClientRect(&rc);//rc为控件的大小。
    StretchDIBits(dc->GetSafeHdc(),0,0,rc.Width(),rc.Height(),0,0,iWidth,iHeight,Image,&bmi,DIB_RGB_COLORS,SRCCOPY);

    delete[] Image;

    //用时
//    clockEnd = clock();
//     int time = (clockEnd-clockBegin);
//         SetDlgItemInt(IDC_EDIT_RECMSG, time);

    //////////////////////////////////////
    free(pData);


}

void CUSBDlg::CmdDrawRawImage(U8 *revbuf,UINT length){
#define imagewidth 160
#define imageheight 160
    static uint16_t revall;
    static uint16_t imagept=0;
    uint16_t buflength=0;
    int end=512;//sio_read(Port,buf,600);
    int cx=160;
    int cy=160;
    static uint8_t imageBuf[imagewidth*imageheight];
    CString a="";

	//正常图片的包是26627字节的
	//由很多小包组成,其中包头占19字节,CRC校验占2字节处于包尾
	//if(length>=26626){
		int i,j;
		for(i=0,j=0;i<length;){
			int k=revbuf[i+15]+revbuf[i+16]*256;//包数据部分长度
			i+=19;//跳过包头
			while(k--){
				imageBuf[j++]=revbuf[i++];
			}
			i+=2;//跳过CRC
		}
        /*for(i=0;i<49;i++){
            if(i==48){
                buflength=256;
            }else{
                buflength=528;
            }
            for(j=(i*549+19);j<(i*549+buflength+19);j++){
                imageBuf[imagept++]=*(revbuf+j);
            }
        }
        imagept=0;*/
        for(i=0;i<160;i++){
            j=i*160;
            imageBuf[j+47]=(imageBuf[j+46]+imageBuf[j+48])>>1;
            imageBuf[j+99]=(imageBuf[j+98]+imageBuf[j+100])>>1;
            imageBuf[j+151]=(imageBuf[j+150]+imageBuf[j+152])>>1;
        }
        //a.Format("%X",imagept);
        //SetDlgItemText(IDC_EDIT_RECMSG,a);
        saveimage(imagewidth,imageheight,imageBuf);
        end=0;
    //}
}

UINT  RevRawImage(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    //int StartTime=GetTickCount();

    pWnd->RevRawImageThread();

    return 1;
}
LRESULT CUSBDlg::OnUpdateData(WPARAM wParam,LPARAM lParam){
    UpdateData(wParam);
    return 0;
}

LRESULT CUSBDlg::RevRawImageThread(void){
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    //	DWORD	w_dwTimeOut = 5000;//30000
#ifdef _DEBUG
    DWORD	w_dwTimeOut=10000;//
#else
    DWORD	w_dwTimeOut=5000;//30000
#endif
    BYTE	g_rPacket[30000];
    uint8_t  *SendPt=NULL;
    m_pThread=TRUE;
    g_Serial.Purge();
    SendCommand(CMD_GET_RAW_IMAGE,SendPt,0);

    w_nResult=g_Serial.Read(g_rPacket,MAX_DATA_LEN,&w_nAckCnt,NULL,w_dwTimeOut);

    // int k;
    if(m_pThread==TRUE){
        if(ERROR_SUCCESS!=w_nResult){
            //((CEdit *)GetDlgItem(IDC_INFO))->SetDlgItemText("接收数据超时");//	
        //	m_strResultMsg=_T("接收数据超时");
            IDCINFORefresh("接收数据超时");
        } else{
            //	m_strResultMsg=_T("数据接收成功");
            IDCINFORefresh("数据接收成功");
            CmdDrawRawImage(g_rPacket,26627);
        }
        //更新编辑框内容
//	m_ctrlBtnGetRawImage.EnableWindow(TRUE);
        OpEnable(BtnRawImageEnd);
    }
    //UpdateData(FALSE); 
    SendMessage(WM_UPDATEDATA,FALSE);
    return 0;
}


LRESULT CUSBDlg::RevModelParamThread(void){
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;

#ifdef _DEBUG
    DWORD	w_dwTimeOut=10000;//
#else
    DWORD	w_dwTimeOut=2000;//30000
#endif
    BYTE	g_rPacket[30000];
    uint8_t  *SendPt=NULL;
    m_pThread=TRUE;
    g_Serial.Purge();

	SendCommand(CMD_GET_RAW_IMAGE,SendPt,0);

    w_nResult=g_Serial.Read(g_rPacket,MAX_DATA_LEN,&w_nAckCnt,NULL,w_dwTimeOut);
	commMsgListen=true;
    // int k;
    if(m_pThread==TRUE){
        if(ERROR_SUCCESS!=w_nResult){
            //IDCINFORefresh("接收数据超时");
        } else{
            //IDCINFORefresh("数据接收成功");
            CmdDrawRawImage(g_rPacket,w_nAckCnt);
        }
        /*更新编辑框内容*/
        OpEnable(BtnRawImageEnd);
    }
    //UpdateData(FALSE); 
    SendMessage(WM_UPDATEDATA,FALSE);
    return 0;
}

UINT AFX_CDECL RevRawImageFirst(LPVOID lParam){
    CUSBDlg *pWnd=(CUSBDlg *)CWnd::FromHandle((HWND)lParam);
    //int StartTime=GetTickCount();
    pWnd->RevModelParamThread();
    //pWnd->RevRawImageThread();

    return 1;
}


void CUSBDlg::OnBtnGetRawImage(){
    uint8_t  *SendPt=NULL;
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=300;//3000
    extern BYTE	g_Packet[];

    IDCINFORefresh("正在读取原始图像......");
    g_Serial.Purge();
    OpEnable(BtnRawImageStart);//更新编辑框内容
	m_hSleepThread=AfxBeginThread(RevRawImageFirst,(LPVOID)this);//启动新的线程
	if(m_hSleepThread==0){
		RevModelParamThread();
	}
}

void CUSBDlg::OnDraw(){

}

void CUSBDlg::OnOK(){

}

void CUSBDlg::OnBtnRdReg(){
    CString a;
    U8 AddrHex=0;
    GetDlgItemText(IDC_EdRdAddr,a);
    m_ctrlBtnRdReg.EnableWindow(FALSE);
    AddrHex=Asc2Hex(a);
    uint8_t  *SendPt=NULL;
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=3000;
    extern BYTE	g_Packet[];
    g_Serial.Purge();

    SendCommand(CMD_READ_NOTE_BOOK,&AddrHex,1);

    w_nResult=g_Serial.Read(g_Packet,22,&w_nAckCnt,NULL,1000);
    if(ERROR_SUCCESS!=w_nResult){
        IDCINFORefresh("寄存器读取失败");
    } else{
        IDCINFORefresh("寄存器读取完成");
        a.Format("%02X",g_Packet[19]);
        SetDlgItemText(IDC_EdRdValue,a);
    }
    m_ctrlBtnRdReg.EnableWindow(TRUE);
}

void CUSBDlg::OnBtnWrReg(){
    CString a;
    U8 RegAdrVal[2];
    GetDlgItemText(IDC_EdWrAddr,a);
    RegAdrVal[0]=Asc2Hex(a);
    GetDlgItemText(IDC_EdWrValue,a);
    RegAdrVal[1]=Asc2Hex(a);
    m_ctrlBtnWrReg.EnableWindow(FALSE);
    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    m_strResultMsg="设置完成";

    m_ctrlBtnWrReg.EnableWindow(TRUE);
}

void timetest(void){
    uint8_t s=0;
    s++;

}

void CUSBDlg::OnBtnIdle(){
    uint8_t  *SendPt=NULL;
    U16 CMD;
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=1000;
    BYTE	g_rPacket[2048];
    m_ctrlBtnGoIdle.EnableWindow(FALSE);
    m_pThread=FALSE;
    if(m_hSleepThread!=NULL){
        SuspendThread(m_hSleepThread);
        TerminateThread(m_hSleepThread,0);
        m_hSleepThread=NULL;
    }
    g_Serial.Purge();
    SendCommand(CMD_SleepToIdle,SendPt,0);
    SendCommand(CMD_SleepToIdle,SendPt,0);

    w_nResult=g_Serial.Read(g_rPacket,22,&w_nAckCnt,NULL,w_dwTimeOut);
    CMD=(g_rPacket[13]<<8)+g_rPacket[12];


    if(w_nResult!=ERROR_SUCCESS){
       IDCINFORefresh("通信超时");

    } else{
        IDCINFORefresh("传感器进入空闲模式");
    }
    g_Serial.Purge();
    OpEnable(BtnIdelModEnd);
}

void CUSBDlg::OnBtnBoardReset(){
    uint8_t  *SendPt=NULL;
    SendCommand(CMD_DEVICE_RESET,SendPt,0);
    GetDlgItem(IDC_BtnGoSleep)->EnableWindow(TRUE);
}

UINT  RevSleepImageThread(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    //int StartTime=GetTickCount();
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=10000;
    BYTE	g_rPacket[30000];
    pWnd->m_pThread=TRUE;
    uint8_t  *SendPt=NULL;
    g_Serial.Purge();
    ///////////////////////////////////////////
    pWnd->m_pThread=TRUE;
    int		w_nRecvLen,w_nTotalRecvLen;
    w_nRecvLen=26627;
    w_nTotalRecvLen=0;
    SendCommand(CMD_Sleep_MODE,SendPt,0);

    while(w_nTotalRecvLen<26627){


        SendCommand(CMD_Sleep_MODE,0,0);
        w_nTotalRecvLen=0;
        g_Serial.Purge();

        w_nResult=g_Serial.Read(&g_rPacket[w_nTotalRecvLen],w_nRecvLen,&w_nAckCnt,NULL,w_dwTimeOut);
        w_nRecvLen=w_nRecvLen-w_nAckCnt;
        w_nTotalRecvLen=w_nTotalRecvLen+w_nAckCnt;
        if(pWnd->m_pThread!=TRUE){
            break;
        }
    }
    //////////////////////////////////////////
    if(pWnd->m_pThread==TRUE){

        pWnd->m_ctrlBtnGoIdle.EnableWindow(FALSE);
        if(w_nTotalRecvLen>=26627){
            pWnd->CmdDrawRawImage(g_rPacket,26627); //要执行的函数
            //	pWnd->m_ctrlBtnGoSleep.EnableWindow(TRUE);
            //pWnd->m_strResultMsg=_T("数据接收成功");
            pWnd->IDCINFORefresh("数据接收成功");
            //			pWnd->UpdateData(FALSE);
        }

        if(pWnd->m_hSleepThread!=NULL){
            pWnd->OpEnable(BtnFingerDetEnd);
            SuspendThread(pWnd->m_hSleepThread);
            TerminateThread(pWnd->m_hSleepThread,0);
            pWnd->m_hSleepThread=NULL;
        }
    }
    //if((LastTime-StartTime)>1500)
    return 1;
}

void CUSBDlg::OnBtnGoSleep(){
    IDCINFORefresh("手指探测模式，请将手指放在传感器上");
    OpEnable(BtnFingerDetStart);
	SendCommand(CMD_DETECT_FINGER,0,0);
    m_hSleepThread=AfxBeginThread(RevSleepImageThread,(LPVOID)this);
}




///////////////////////////////////////////////

void CUSBDlg::TraversalCom(void){
    EnumerateSerialPorts(ports,portse,portsu);
    unsigned short Counter;
    unsigned short Setcom;
    CString str;

    //获取可用串口个数
    Counter=ports.GetSize();
    //如果个数大于0
    if(Counter>0){
        //初始化串口列表框
        for(int i=0; i<Counter; i++){
            Setcom=ports.ElementAt(i);
            str.Format(_T("COM%d"),Setcom);
            ((CComboBox *)GetDlgItem(IDC_CBComPort))->InsertString(i,str);
        }
		((CComboBox *)GetDlgItem(IDC_CBComPort))->SetCurSel(0);
    }
}


void CUSBDlg::EnumerateSerialPorts(CUIntArray& ports,CUIntArray& portse,CUIntArray& portsu){
    //清除串口数组内容
    ports.RemoveAll();
    portse.RemoveAll();
    portsu.RemoveAll();
    int portState=0;
    //因为至多有255个串口，所以依次检查各串口是否存在
    //如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，
    //都认为串口存在，只不过后者表明串口已经被占用,否则串口不存在
    for(int i=1; i<30; i++){
        //生成原始设备名
        CString sPort;
        sPort.Format(_T("\\\\.\\COM%d"),i);

        //试图打开此串口
        BOOL bSuccess;
        HANDLE hPort=::CreateFile(sPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
        if(hPort==INVALID_HANDLE_VALUE){
			//打开串口失败
			bSuccess=FALSE;
            DWORD dwError=GetLastError();
            if(dwError==ERROR_ACCESS_DENIED){
                bSuccess=TRUE;
                portsu.Add(i);//已占用的串口
            }
        } else{
            //打开串口成功
            bSuccess=TRUE;
            portse.Add(i);//可用的串口
            if(!CloseHandle(hPort)){
                DWORD dwError=GetLastError();//关闭不成功的原因
            }
        }
        if(bSuccess){
            ports.Add(i);//所有存在的串口
        }
    }
}

UINT  myproc(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    pWnd->ReDetectCom();                         //要执行的函数
    return 1;
}


BOOL CUSBDlg::OnDeviceChange(UINT nEventType,DWORD dwData){
    switch(nEventType){
        case DBT_DEVICEREMOVECOMPLETE://移除设备
            SetDlgItemText(IDC_BtnPortOpen,"打开");
            g_Serial.Close();
            OpEnable(BtnOpen);
        case DBT_DEVICEARRIVAL://添加设备
        	ReDetectCom();//刷新列表框的内容
            m_hSleepThread=AfxBeginThread(myproc,(LPVOID)this);//启动新的线程
            break;
        default:
            break;
    }
    return TRUE;
}

void CUSBDlg::ReDetectCom(void){
    ((CComboBox *)GetDlgItem(IDC_CBComPort))->ResetContent();
    TraversalCom();
}


void CUSBDlg::OnPortopen(){
    int i=0;
    m_ctrlPortOpen.EnableWindow(FALSE);
    IDCINFORefresh("正在操作串口");

    i=((CComboBox *)GetDlgItem(IDC_CBComPort))->GetCurSel();
    BaudRate=GetDlgItemInt(IDC_CBComBaudRate);
    CString a;
    GetDlgItemText(IDC_BtnPortOpen,a);
    SoftParam.ComPort=ports.ElementAt(i);
    if(!strcmp(a,"打开")){
        if(OpenSerialPort(SoftParam.ComPort,BaudRate)!=TRUE){
            MessageBox("串口打开错误");
        } else{
            SetDlgItemText(IDC_BtnPortOpen,"关闭");
            IDCINFORefresh("串口已打开");
            g_Serial.Purge();
            m_hSleepThread=AfxBeginThread(RevRawImage,(LPVOID)this);//启动新的线程

            OpEnable(BtnClose);//更新编辑框内容
        }
    } else{
        m_pThread=FALSE;
        if(m_hSleepThread!=NULL){
            SuspendThread(m_hSleepThread);
            TerminateThread(m_hSleepThread,0);
            m_hSleepThread=NULL;
        }
        SetDlgItemText(IDC_BtnContinuousGetImage,"连续获取图像");
        g_Serial.Close();
        OpEnable(BtnOpen);
        SetDlgItemText(IDC_BtnPortOpen,"打开");
        IDCINFORefresh("串口已关闭");
    }
    m_ctrlPortOpen.EnableWindow(TRUE);
}

void CUSBDlg::OpEnable(BtnOpEnable NO){
    switch(NO){
        case BtnRawImageEnd:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnContinuGetImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
        case BtnRawImageStart:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnContinuGetImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;
        case BtnFingerDetEnd:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnContinuGetImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
        case BtnFingerDetStart:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(TRUE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnContinuGetImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;
        case BtnIdelModEnd:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnContinuGetImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
        case BtnIdelModStart:
            break;
        case BtnAdjustImageEnd:
            break;
        case BtnAdjustImageStart:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnContinuGetImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;
        case BtnContinuesGetImageEnd:
            break;
        case BtnContinuesGetImageStart:
            break;
        case BtnLaunchStateEnd:

            break;
        case BtnLaunchStateStart:   break;


        case 	BtnClose:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlPortOpen.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnContinuGetImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
        case 	BtnOpen:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlPortOpen.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnContinuGetImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;

        case BtnContinuousGetImageStart:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlPortOpen.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;
        case BtnContinuousGetImageEnd:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlPortOpen.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
            //m_ctrlBtnContinuGetImage.EnableWindow(TRUE);
        case BtnBlackWhiteEnd:
            m_ctrlBtnGetRawImage.EnableWindow(TRUE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(TRUE);
            m_ctrlBtnRdReg.EnableWindow(TRUE);
            m_ctrlBtnWrReg.EnableWindow(TRUE);
            m_ctrlPortOpen.EnableWindow(TRUE);
            m_ctrlBtnAdjustImage.EnableWindow(TRUE);
            m_ctrlBtnBlackWhite.EnableWindow(TRUE);
            break;
        case BtnBlackWhiteStart:
            m_ctrlBtnGetRawImage.EnableWindow(FALSE);
            m_ctrlBtnGoIdle.EnableWindow(FALSE);
            m_ctrlBtnGoSleep.EnableWindow(FALSE);
            m_ctrlBtnRdReg.EnableWindow(FALSE);
            m_ctrlBtnWrReg.EnableWindow(FALSE);
            m_ctrlPortOpen.EnableWindow(FALSE);
            m_ctrlBtnAdjustImage.EnableWindow(FALSE);
            m_ctrlBtnBlackWhite.EnableWindow(FALSE);
            break;
        case BtnDelAllEnrollStart:

            break;

        case BtnDelAllEnrollEnd:

            break;
    }
    // 			m_ctrlBtnGetRawImage.EnableWindow(op);
    // 			m_ctrlBtnGoIdle.EnableWindow(op);
    // 			m_ctrlBtnGoSleep.EnableWindow(op);
    // 			m_ctrlBtnRdReg.EnableWindow(op);
    // 			m_ctrlBtnWrReg.EnableWindow(op);
    // 			m_ctrlBtnAdjustImage.EnableWindow(op);
    //	UpdateData(FALSE);
}

BOOL CUSBDlg::OpenSerialPort(U8 ComPort,int ComBaudRate){
    CString	w_strComPort;
    LONG    w_lRetCode=ERROR_SUCCESS;

    //	ASSERT( m_nComPortIndex >= 0); //如果不成立，系统弹出一个对话框警告你，并停止程序的执行。

        //GetLBText()函数.其原型为void GetLBText( int nIndex, CString&rString ).nIndex为列表框中数据的索引值,rString 为字符串,
        //其作用就是把索引号为nIndex的数据放到rString变量中.与其对应的GetLBTextLen(int nIndex)函数就是得到索引号为nIndex中数据的长度
        //m_CtrlComboPort.GetLBText(m_nComPortIndex, w_strComPort);

    //	w_strComPort = _T("\\\\.\\") + w_strComPort; //格式的转换
    w_strComPort.Format(_T("\\\\.\\COM%d"),ComPort);
    //	ASSERT(m_nBaudRateIndex >= 0);
    //	ASSERT(m_nBaudRateIndex <= BAUD460800);

    //. Attempt to open the serial port (COM1)
    w_lRetCode=g_Serial.Open(w_strComPort,2048,2048,true);
    if(w_lRetCode!=ERROR_SUCCESS){
        return FALSE;
    }

    //. Setup the serial port (9600,8N1, which is the default setting)
    w_lRetCode=g_Serial.Setup((CSerial::EBaudrate)ComBaudRate,CSerial::EData8,CSerial::EParNone,CSerial::EStop1);
    if(w_lRetCode!=ERROR_SUCCESS){
        return FALSE;
    }

    //. Register only for the receive event
    w_lRetCode=g_Serial.SetMask(CSerial::EEventRecv);
    if(w_lRetCode!=ERROR_SUCCESS){
        return FALSE;
    }
    return TRUE;
}
/***************************************************************************/
/***************************************************************************/
bool	ReadDataN(BYTE* p_pData,int p_nLen,DWORD p_dwTimeOut){
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    int		w_nRecvLen,w_nTotalRecvLen;

    w_nRecvLen=p_nLen;
    w_nTotalRecvLen=0;

    while(w_nTotalRecvLen<p_nLen){
        w_nResult=g_Serial.Read(&p_pData[w_nTotalRecvLen],w_nRecvLen,&w_nAckCnt,NULL,p_dwTimeOut);

        if(ERROR_SUCCESS!=w_nResult){
            return FALSE;
        }

        w_nRecvLen=w_nRecvLen-w_nAckCnt;
        w_nTotalRecvLen=w_nTotalRecvLen+w_nAckCnt;
    }

    return TRUE;
}
/***************************************************************************/
LRESULT CUSBDlg::RevAdjuestRawImageThread(void){
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
#ifdef _DEBUG
    DWORD	w_dwTimeOut=10000;//
#else
    DWORD	w_dwTimeOut=5000;//30000
#endif
    BYTE	g_rPacket[30000];
    uint8_t  *SendPt=NULL;

    g_Serial.Purge();

    SendCommand(CMD_AdjustImage,SendPt,0);
    //m_strResultMsg=_T("正在读取修正图像......");
    IDCINFORefresh("正在读取修正图像......");
    w_nResult=g_Serial.Read(g_rPacket,MAX_DATA_LEN,&w_nAckCnt,NULL,w_dwTimeOut);

    // int k;
    if(m_pThread==TRUE){
        if(ERROR_SUCCESS!=w_nResult){
            //((CEdit *)GetDlgItem(IDC_INFO))->SetDlgItemText("接收数据超时");//	
        //	m_strResultMsg=_T("接收数据超时");
            IDCINFORefresh("接收数据超时");
        } else{
            //	m_strResultMsg=_T("数据接收成功");
            IDCINFORefresh("数据接收成功");
            CmdDrawRawImage(g_rPacket,26627);
        }
        //更新编辑框内容
        //	m_ctrlBtnAdjustImage.EnableWindow(TRUE);
        OpEnable(BtnRawImageEnd);
    }
    //UpdateData(FALSE); 
    SendMessage(WM_UPDATEDATA,FALSE);
    return 0;
}

UINT  RevAdjuestRawImage(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    //int StartTime=GetTickCount();
    pWnd->RevAdjuestRawImageThread();
    return 1;
}
void CUSBDlg::OnAdjustImage(){
    uint8_t  *SendPt=NULL;
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=300;//3000
    extern BYTE			g_Packet[];

    OpEnable(BtnAdjustImageStart);
    //m_strResultMsg = _T("正在读取修正图像......");
	IDCINFORefresh("正在读取修正图像......");
	//SetDlgItemText(IDC_INFO ,"");
	//UpdateData(FALSE);           //更新编辑框内容
    g_Serial.Purge();

    m_hSleepThread=AfxBeginThread(RevAdjuestRawImage,(LPVOID)this);//启动新的线程
}




LRESULT CUSBDlg::ContinuousRevSleepImageThread(void){
    //	CUSBDlg *pWnd = (CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
        //int StartTime=GetTickCount();
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    DWORD	w_dwTimeOut=10000;
    BYTE	g_rPacket[30000]={0};

    m_pThread=TRUE;
    uint8_t  *SendPt=NULL;
    g_Serial.Purge();
    ///////////////////////////////////////////
    m_pThread=TRUE;
    int		w_nRecvLen,w_nTotalRecvLen;
    w_nRecvLen=26627;
    w_nTotalRecvLen=0;
    SendCommand(CMD_Sleep_MODE,SendPt,0);
    IDCINFORefresh("请放手指......");
    while(w_nTotalRecvLen<26627){
        if(w_nTotalRecvLen==0){
            g_Serial.Purge();
            SendCommand(CMD_Sleep_MODE,0,0);
            w_nTotalRecvLen=0;

        }
        w_nResult=g_Serial.Read(&g_rPacket[w_nTotalRecvLen],w_nRecvLen,&w_nAckCnt,NULL,w_dwTimeOut);

        w_nRecvLen=w_nRecvLen-w_nAckCnt;
        w_nTotalRecvLen=w_nTotalRecvLen+w_nAckCnt;
        if(m_pThread!=TRUE){
            break;
        }
    }
    //////////////////////////////////////////
    if(m_pThread==TRUE){

        m_ctrlBtnGoIdle.EnableWindow(FALSE);
        if(w_nTotalRecvLen>=26627){
            CmdDrawRawImage(g_rPacket,26627); //要执行的函数
            //	pWnd->m_ctrlBtnGoSleep.EnableWindow(TRUE);
            //pWnd->m_strResultMsg=_T("数据接收成功");
            IDCINFORefresh("数据接收成功");
            //			pWnd->UpdateData(FALSE);
        }

        // 		if(m_hSleepThread != NULL)
        // 		{	
        // 			OpEnable(BtnFingerDetEnd);	
        // 			SuspendThread(m_hSleepThread);
        // 			TerminateThread(m_hSleepThread, 0);
        // 			m_hSleepThread = NULL;
        // 		}
    }
    //if((LastTime-StartTime)>1500)
    return 1;
}

UINT  ContinuousRevSleepImage(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)CWnd::FromHandle((HWND)lParam);
    while(pWnd->m_pThread){
        pWnd->RevModelParamThread();
    }
    return 1;
}

void CUSBDlg::OnBtnContinuousGetImage(){
    CString a;
    GetDlgItemText(IDC_BtnContinuousGetImage,a);
    if(!strcmp(a,"连续获取图像")){
        SetDlgItemText(IDC_BtnContinuousGetImage,"停止连续获取");
        IDCINFORefresh("正在连续读取图像......");
        OpEnable(BtnContinuousGetImageStart);
        m_pThread=TRUE;
        m_hSleepThread=AfxBeginThread(ContinuousRevSleepImage,(LPVOID)this);
    } else{
        m_pThread=FALSE;
        if(m_hSleepThread!=NULL){
            SuspendThread(m_hSleepThread);
            TerminateThread(m_hSleepThread,0);
            m_hSleepThread=NULL;
        }
        SetDlgItemText(IDC_BtnContinuousGetImage,"连续获取图像");
        IDCINFORefresh("停止连续读取图像......");

        OpEnable(BtnContinuousGetImageEnd);
    }
}

void CUSBDlg::IDCINFORefresh(CString str){
    InfoStr[3]=InfoStr[2];
    InfoStr[2]=InfoStr[1];
    InfoStr[1]=InfoStr[0];
    InfoStr[0]=str+"\r\n";
    SetDlgItemText(IDC_INFO,InfoStr[0]+InfoStr[1]+InfoStr[2]+InfoStr[3]);
    UpdateData(TRUE);
}

LRESULT CUSBDlg::ReadMaxBlackWhiteImageThread(void){
    //	CUSBDlg *pWnd = (CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    //int StartTime=GetTickCount();

    uint8_t RegAdrVal[2];



    Sleep(100);
    RegAdrVal[0]=0x07;
    RegAdrVal[1]=0x23;

    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    Sleep(200);	///////////////////////////////////////////
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    //	DWORD	w_dwTimeOut = 5000;//30000
#ifdef _DEBUG
    DWORD	w_dwTimeOut=10000;//
#else
    DWORD	w_dwTimeOut=5000;//30000
#endif
    BYTE	g_rPacket[30000];
    uint8_t  *SendPt=NULL;
    m_pThread=TRUE;
    g_Serial.Purge();
    SendCommand(CMD_GET_RAW_IMAGE,SendPt,0);

    w_nResult=g_Serial.Read(g_rPacket,MAX_DATA_LEN,&w_nAckCnt,NULL,w_dwTimeOut);

    // int k;
    if(m_pThread==TRUE){
        if(ERROR_SUCCESS!=w_nResult){
            //((CEdit *)GetDlgItem(IDC_INFO))->SetDlgItemText("接收数据超时");//	
            //	m_strResultMsg=_T("接收数据超时");
            IDCINFORefresh("接收数据超时");
        } else{
            //	m_strResultMsg=_T("数据接收成功");
            IDCINFORefresh("数据接收成功");
            CmdDrawRawImage(g_rPacket,26627);
        }
        //更新编辑框内容
        //	m_ctrlBtnGetRawImage.EnableWindow(TRUE);
        OpEnable(BtnRawImageEnd);
    }
    //UpdateData(FALSE); 
    SendMessage(WM_UPDATEDATA,FALSE);
    g_Serial.Purge();
    RegAdrVal[0]=0x07;
    RegAdrVal[1]=0x1F;

    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    OpEnable(BtnBlackWhiteEnd);

    return 1;
}
UINT  ReadMaxBlackWhiteImage(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针

    pWnd->ReadMaxBlackWhiteImageThread();

    return 1;
}
void CUSBDlg::OnBlackWhite(){
    m_pThread=TRUE;
    uint8_t RegAdrVal[2];
    RegAdrVal[0]=0x08;
    RegAdrVal[1]=0x03;
    OpEnable(BtnBlackWhiteStart);
    g_Serial.Purge();
    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    m_hSleepThread=AfxBeginThread(ReadMaxBlackWhiteImage,(LPVOID)this);//启动新的线程
}

LRESULT CUSBDlg::RecodeDelThread(void){
    //	CUSBDlg *pWnd = (CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    //int StartTime=GetTickCount();
    uint8_t RegAdrVal[2];
    Sleep(100);
    RegAdrVal[0]=0x07;
    RegAdrVal[1]=0x23;

    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    Sleep(200);	///////////////////////////////////////////
    DWORD	w_nAckCnt=0;
    LONG	w_nResult=0;
    //	DWORD	w_dwTimeOut = 5000;//30000
#ifdef _DEBUG
    DWORD	w_dwTimeOut=10000;//
#else
    DWORD	w_dwTimeOut=5000;//30000
#endif
    BYTE	g_rPacket[30000];
    uint8_t  *SendPt=NULL;
    m_pThread=TRUE;
    g_Serial.Purge();
    SendCommand(CMD_GET_RAW_IMAGE,SendPt,0);

    w_nResult=g_Serial.Read(g_rPacket,MAX_DATA_LEN,&w_nAckCnt,NULL,w_dwTimeOut);

    // int k;
    if(m_pThread==TRUE){
        if(ERROR_SUCCESS!=w_nResult){
            //((CEdit *)GetDlgItem(IDC_INFO))->SetDlgItemText("接收数据超时");//	
            //	m_strResultMsg=_T("接收数据超时");
            IDCINFORefresh("接收数据超时");
        } else{
            //	m_strResultMsg=_T("数据接收成功");
            IDCINFORefresh("数据接收成功");
            CmdDrawRawImage(g_rPacket,26627);
        }
        //更新编辑框内容
        //	m_ctrlBtnGetRawImage.EnableWindow(TRUE);
        OpEnable(BtnRawImageEnd);
    }
    //UpdateData(FALSE); 
    SendMessage(WM_UPDATEDATA,FALSE);
    g_Serial.Purge();
    RegAdrVal[0]=0x07;
    RegAdrVal[1]=0x1F;

    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    OpEnable(BtnBlackWhiteEnd);

    return 1;
}

UINT  RecodeDel(LPVOID  lParam){
    CUSBDlg *pWnd=(CUSBDlg *)lParam;         //将窗口指针赋给无类型指针
    pWnd->ReadMaxBlackWhiteImageThread();
    return 1;
}

void CUSBDlg::OnBtnRecodeDel(){
    m_pThread=TRUE;
    uint8_t RegAdrVal[2];
    RegAdrVal[0]=0x08;
    RegAdrVal[1]=0x03;
    OpEnable(BtnDelAllEnrollStart);
    g_Serial.Purge();
    SendCommand(CMD_WRITE_NOTE_BOOK,RegAdrVal,2);
    m_hSleepThread=AfxBeginThread(RecodeDel,(LPVOID)this);//启动新的线程
}

void CUSBDlg::OnClickListRecord(NMHDR* pNMHDR,LRESULT* pResult){
    *pResult=0;
    LPNMLISTVIEW pNMLV=reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    int index=pNMLV->iItem;
    if(index==-1)
        return;
    HMENU hMenu=::CreatePopupMenu();
    AppendMenu(hMenu,MF_STRING,10001,_T("菜单项1"));
    CPoint pt;
    GetCursorPos(&pt); //获得当前鼠标位置
    UINT Cmd=(UINT)::TrackPopupMenu(hMenu,TPM_LEFTALIGN|TPM_RETURNCMD,pt.x,pt.y,0,m_hWnd,NULL);//弹出菜单
    switch(Cmd)//响应点击的菜单
    {
        case 10001:
            break;
    }
}