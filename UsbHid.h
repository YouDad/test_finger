// UsbHid.h: interface for the UsbHid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBHID_H__CF9D3B20_3559_4825_8087_F1655E4DFD5B__INCLUDED_)
#define AFX_USBHID_H__CF9D3B20_3559_4825_8087_F1655E4DFD5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define USBD_VID                     0x1183
#define USBD_PID                     0x5711



class CUsbHid  
{
public:
	bool ReadHid(BYTE *str, DWORD *len, DWORD *err);
	bool WriteHid(BYTE *str, DWORD *len, DWORD *err);
	void CloseHid(void);
	CUsbHid();
	virtual ~CUsbHid();
	bool OpenHid(USHORT VID, USHORT PID);
private:
	//用来保存读数据的设备句柄
	HANDLE hReadHandle;
	//用来保存写数据的设备句柄
	HANDLE hWriteHandle;
};

#endif // !defined(AFX_USBHID_H__CF9D3B20_3559_4825_8087_F1655E4DFD5B__INCLUDED_)
