// CEnrollDlg.cpp : implementation file
//

#include "stdafx.h"
#include "usb.h"
#include "CEnrollDlg.h"
#include "USBDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
BOOL CEnrollDlg::OnInitDialog(){
    CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    m_ctrlCbOption.SetCurSel(0);
    m_ctrlCbRole.SetCurSel(0);
    //	all_struct_Init();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

CEnrollDlg::CEnrollDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CEnrollDlg::IDD,pParent){
    //{{AFX_DATA_INIT(CEnrollDlg)

//m_ctrlCbOption.InsertString(0,_T("None"));
// 	((CComboBox *)GetDlgItem(IDC_ComboxRole))->SetCurSel(2);
//m_ctrlCbOption.AddString("nONE");

// 	m_CbRole.Insert(0,"Guest");
// 	m_CbRole.Insert(1,"Normal");
// 	m_CbRole.Insert(2,"Admin");
//	m_CbRole.selectedindex=2;
//	m_ctrlCbRole.AddString("None\nAuto ID\nAdd New\nCheck ID\nCheck Finger\n");
//UpdateWindow();
    //}}AFX_DATA_INIT
 //	m_CbOption.selec (1);

// 	((CComboBox *)GetDlgItem(IDC_ComboxOption))->SetCurSel(1);
// 	((CComboBox *)GetDlgItem(IDC_ComboxRole))->SetCurSel(2);
}


void CEnrollDlg::DoDataExchange(CDataExchange* pDX){
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEnrollDlg)
    DDX_Control(pDX,IDC_ComboxRole,m_ctrlCbRole);
    DDX_Control(pDX,IDC_ComboxOption,m_ctrlCbOption);
    DDX_Text(pDX,IDC_EdTemplateNo,m_ValueTemplateNO);
    DDX_CBString(pDX,IDC_ComboxOption,m_CbOption);
    DDX_CBString(pDX,IDC_ComboxRole,m_CbRole);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnrollDlg,CDialog)
    //{{AFX_MSG_MAP(CEnrollDlg)
    ON_WM_KEYDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnrollDlg message handlers

void CEnrollDlg::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags){
    // TODO: Add your message handler code here and/or call default

    CDialog::OnKeyDown(nChar,nRepCnt,nFlags);
}
//_XFA_Param dlg::xFA_Param;
void CEnrollDlg::OnOK(){
    // TODO: Add extra validation here
    //	PostMessageW(GetParent()->m_hWnd,WM_QUIT,0,0);
    // 	CUSBDlg USBDlg;
    CString a;
    int b;

    GetDlgItemText(IDC_TemplateNo,a);
    b=atoi(a);
    //m_ValueTemplateNO
    // 	GetDlgItemInt(IDC_TemplateNo,a);
    UserEnrollInFO.NO=b;
    UserEnrollInFO.Option=m_ctrlCbOption.GetCurSel();
    UserEnrollInFO.Pole=m_ctrlCbRole.GetCurSel();
    //	xFA_Param.Arith_SaveNo=b;
       // TODO: Add your control notification handler code here
   //	delete this;
    CDialog::OnOK();
    //	CUSBApp::BaudRate=0;
}

void CEnrollDlg::OnCancel(){
    // TODO: Add extra cleanup here
//	delete this;
    CDialog::OnCancel();
}
