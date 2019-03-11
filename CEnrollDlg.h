#if !defined(AFX_CENROLLDLG_H__5FF46E36_25AA_4F9E_8355_69D2055FB695__INCLUDED_)
#define AFX_CENROLLDLG_H__5FF46E36_25AA_4F9E_8355_69D2055FB695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEnrollDlg.h : header file
//
#include "protocol.h"
/////////////////////////////////////////////////////////////////////////////
// CEnrollDlg dialog
struct _EnrollDialogParam{
    UINT16 NO;
    UINT16 Option;
    UINT16 Pole;

};

typedef struct{
    int 			Arith_LegitimacySign;
    unsigned char 	Arith_sn[16];
    char			Arith_Version[32];
    int 			Arith_FingerCount;
    uint8_t 		Arith_MatchBZ;
    uint8_t		Arith_EnrollS;
    uint8_t		Arith_License[256];
    UINT8			Arith_SaveNo;
    UINT8			Arith_EnrollState;

}_XFA_Param;

class CEnrollDlg: public CDialog{
    // Construction
public:
    CEnrollDlg(CWnd* pParent=NULL);   // standard constructor
    int		UserNO;
    struct _EnrollDialogParam	UserEnrollInFO;
    static	 _XFA_Param xFA_Param;
    // Dialog Data
        //{{AFX_DATA(CEnrollDlg)
    enum{
        IDD=IDD_Enroll_DIALOG
    };
    CComboBox	m_ctrlCbRole;
    CComboBox	m_ctrlCbOption;
    int		m_ValueTemplateNO;
    CString	m_CbOption;
    CString	m_CbRole;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEnrollDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEnrollDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnInitDlg();
    afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CENROLLDLG_H__5FF46E36_25AA_4F9E_8355_69D2055FB695__INCLUDED_)
