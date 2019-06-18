// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
#pragma once

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#define STRICT

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//C
#include <afxpriv.h>
#include <Dbt.h>
#include <assert.h>
#include <io.h>
//C++
#include <functional>
#include <iterator>
#include <algorithm>
#include <exception>
#include <vector>
#include <queue>
#include <map>

#include "MyDefine.h"

#include "resource.h"

#include "Test.h"

#include "MyLocker.h"
#include "MyControl.h"
#include "MyConfig.h"
#include "strings.h"
#include "MyThread.h"
#include "MyFlow.h"
#include "MyString.h"
#include "MyLog.h"
#include "MyNet.h"

#include "Serial.h"
#include "enumcd.h"
#include "UsbPort.h"

#include "App.h"
#include "MainDialog.h"
#include "AdvancedDebugDialog.h"
#include "SettingDialog.h"

#include "MainControl.h"
#include "MainDialogCtrlValidity.h"

#include "DataPacket.h"
#include "ConverterBoardcast.h"
#include "MyConverterHeadFile.h"
#include "ListenerBoardcast.h"
#include "MyListenerHeadFile.h"
#include "Comm.h"

#include "MyImage.h"
