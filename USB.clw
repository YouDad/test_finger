; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUSBDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "usb.h"
LastPage=0

ClassCount=5
Class1=CUSBApp
Class2=CAboutDlg
Class3=CUSBDlg

ResourceCount=3
Resource1=IDD_Enroll_DIALOG
Resource2=IDD_ABOUTBOX
Class4=CEnrollDlg
Class5=IDC_TemplateNo
Resource3=IDD_USB_DIALOG

[CLS:CUSBApp]
Type=0
BaseClass=CWinApp
HeaderFile=USB.h
ImplementationFile=USB.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=USBDlg.cpp
ImplementationFile=USBDlg.cpp
Filter=D
VirtualFilter=dWC

[CLS:CUSBDlg]
Type=0
BaseClass=CDialog
HeaderFile=USBDlg.h
ImplementationFile=USBDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CUSBDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_BtnCancel,button,1342242817

[DLG:IDD_USB_DIALOG]
Type=1
Class=CUSBDlg
ControlCount=55
Control1=VS_VERSION_INFO,button,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BtnContinuousGetImage,button,1476460544
Control6=IDC_STATIC,button,1342177287
Control7=IDC_RADIO1,button,1476395017
Control8=IDC_RADIO2,button,1476395017
Control9=IDC_RADIO3,button,1476395017
Control10=IDC_STATIC,button,1342177287
Control11=IDC_CBArithmeticVer,combobox,1478557954
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CBComPort,combobox,1344340226
Control14=IDC_STATIC,static,1342308352
Control15=IDC_CBComBaudRate,combobox,1344340226
Control16=IDC_STATIC,static,1476526080
Control17=IDC_STATIC,static,1476526080
Control18=IDC_DeviceAddrr,edit,1484849280
Control19=IDC_DevicePwd,edit,1484849280
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_BtnRdReg,button,1476460544
Control23=IDC_BtnWrReg,button,1476460544
Control24=IDC_EdRdAddr,edit,1350631552
Control25=IDC_EdWrAddr,edit,1350631552
Control26=IDC_EdRdValue,edit,1350633600
Control27=IDC_EdWrValue,edit,1350631552
Control28=IDC_BtnPortOpen,button,1342242816
Control29=IDC_BtnGetRawImage,button,1476460544
Control30=IDC_IMAGE3,static,1350570510
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_BtnGoSleep,button,1476460544
Control36=IDC_BtnIdle,button,1476460544
Control37=IDC_STATIC,static,1342308352
Control38=IDC_INFO,edit,1476460548
Control39=IDC_STATIC,button,1342177287
Control40=IDC_STATIC,static,1342308352
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC,static,1342308352
Control44=IDC_STATIC,static,1342308352
Control45=IDC_BtnParamReset,button,1476460544
Control46=IDC_EdtManuFacture,edit,1350633600
Control47=IDC_EdtproductModel,edit,1350633600
Control48=IDC_EdtSWVersion,edit,1350633600
Control49=IDC_EdtDeviceAddress,edit,1350633600
Control50=IDC_EdtComBaudRate,edit,1350633600
Control51=IDC_AdjustImage,button,1476460544
Control52=IDC_BUTTON2,button,1476460544
Control53=IDC_ListRecord,SysListView32,1350631425
Control54=IDC_BtnEnroll,button,1342242816
Control55=IDC_BtnIdentify,button,1342242816

[CLS:CEnrollDlg]
Type=0
HeaderFile=CEnrollDlg.h
ImplementationFile=CEnrollDlg.cpp
BaseClass=CDialog
Filter=C
LastObject=IDC_ComboxOption
VirtualFilter=dWC

[DLG:IDD_Enroll_DIALOG]
Type=1
Class=CEnrollDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ComboxRole,combobox,1344340226
Control4=IDC_UserRole,static,1342308352
Control5=IDC_ComboxOption,combobox,1344339971
Control6=IDC_UserID,static,1342308352
Control7=IDC_Option,static,1342308352
Control8=IDC_EdTemplateNo,edit,1350639744

[CLS:IDC_TemplateNo]
Type=0
HeaderFile=IDC_TemplateNo.h
ImplementationFile=IDC_TemplateNo.cpp
BaseClass=CEdit
Filter=W

