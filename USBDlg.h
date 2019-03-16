// USBDlg.h : header file
//

#if !defined(AFX_USBDLG_H__7A9DE3A8_E5EE_4E4B_9790_6A1C0B00BA8A__INCLUDED_)
#define AFX_USBDLG_H__7A9DE3A8_E5EE_4E4B_9790_6A1C0B00BA8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdio.h>
#include "protocol.h"
#include "winuser.h"
#define   WM_UPDATEDATA   WM_USER + 5 
//_XFA_Param xFA_Param;
/////////////////////////////////////////////////////////////////////////////
// CUSBDlg dialog
typedef enum{
    BtnRawImageEnd=0,
    BtnRawImageStart,
    BtnClose,
    BtnOpen,
    BtnFingerDetEnd,
    BtnFingerDetStart,
    BtnIdelModEnd,
    BtnIdelModStart,
    BtnAdjustImageEnd,
    BtnAdjustImageStart,
    BtnContinuesGetImageEnd,
    BtnContinuousGetImageEnd,
    BtnContinuesGetImageStart,
    BtnContinuousGetImageStart,
    BtnLaunchStateEnd,
    BtnLaunchStateStart,
    BtnBlackWhiteEnd,
    BtnBlackWhiteStart,
    BtnDelAllEnrollStart,
    BtnDelAllEnrollEnd,
}BtnOpEnable;

class CUSBDlg: public CDialog{
    // Construction
public:
    CUSBDlg(CWnd* pParent=NULL);// standard constructor
    void ReDetectCom(void);
    void CmdDrawRawImage(U8 *revbuf,UINT length);
    // Dialog Data
    enum{
        IDD=IDD_USB_DIALOG
    };
    CButton	m_ctrlBtnEnroll;
    CListCtrl	m_ctrlListRecord;
    CButton	m_ctrlBtnBlackWhite;
    CButton	m_ctrlBtnContinuGetImage;
    CButton	m_ctrlPortOpen;
    CButton	m_ctrlBtnAdjustImage;
    CButton	m_ctrlBtnWrReg;
    CButton	m_ctrlBtnRdReg;
    CButton	m_ctrlBtnGoIdle;
    CButton	m_ctrlBtnGoSleep;
    CButton	m_ctrlBtnGetRawImage;
    CString	m_strResultMsg;

    // ClassWizard generated virtual function overrides
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual void OnDraw();  // overridden to draw this view
    virtual void saveimage(int width,int height,unsigned char *image);
    virtual void EnumerateSerialPorts(CUIntArray& ports,CUIntArray& portse,CUIntArray& portsu);
    virtual void TraversalCom(void);
	virtual void OnOK();

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID,LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnPortopen();
    afx_msg void OnBtnGetRawImage();
    afx_msg void OnBtnRdReg();
    afx_msg void OnBtnWrReg();
    afx_msg	void OnBtnGoSleep();
    afx_msg	void OnBtnIdle();
    afx_msg BOOL OnDeviceChange(UINT nEventType,DWORD dwData);
    afx_msg void OnBtnBoardReset();
    afx_msg void OnAdjustImage();
    afx_msg void OnBtnContinuousGetImage();
    afx_msg void OnBlackWhite();
    afx_msg void IdListInit();
    afx_msg void OnBtnRecodeDel();
    afx_msg void OnClickListRecord(NMHDR* pNMHDR,LRESULT* pResult);


    DECLARE_MESSAGE_MAP()
public:
    int Port;
    int BaudRate;
    CBitmap m_bmp;
    BOOL	m_pThread;
    struct _soft_param SoftParam;
    HANDLE		m_hSleepThread;
    BOOL OpenSerialPort(U8 Comport,int ComBaudRate);
    bool	ReadDataN(BYTE* p_pData,int p_nLen,DWORD p_dwTimeOut);
    LRESULT RevRawImageThread(void);
    LRESULT RevAdjuestRawImageThread(void);
    LRESULT OnUpdateData(WPARAM wParam,LPARAM lParam);
    //LRESULT ContinuousRevRawImageThread(void);
    LRESULT ContinuousRevSleepImageThread(void);
    LRESULT ReadMaxBlackWhiteImageThread(void);
    LRESULT RecodeDelThread(void);
    LRESULT RevModelParamThread(void);
    void IDCINFORefresh(CString str);
    void OpEnable(BtnOpEnable NO);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBDLG_H__7A9DE3A8_E5EE_4E4B_9790_6A1C0B00BA8A__INCLUDED_)
