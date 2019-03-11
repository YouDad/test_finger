// USB.h : main header file for the USB application
//

#if !defined(AFX_USB_H__98A8E434_52C1_4BA8_A8EA_ECD71AAB3492__INCLUDED_)
#define AFX_USB_H__98A8E434_52C1_4BA8_A8EA_ECD71AAB3492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxctl.h>
#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUSBApp:
// See USB.cpp for the implementation of this class
//

class CUSBApp: public CWinApp{
public:
    CUSBApp();

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CUSBApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CUSBApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USB_H__98A8E434_52C1_4BA8_A8EA_ECD71AAB3492__INCLUDED_)
