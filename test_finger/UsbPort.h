#pragma once

#include "Windows.h"
#include "SetupAPI.h"


extern "C"
{
	#include "hidsdi.h"
};

#define DEVICE_NAME						"YW_FPM_YW606_YW0192"
#define SCSI_TIMEOUT 5

#define IOCTRL_CDB_LEN	16

class CUsbPort
{
public:
	CUsbPort(void);
	~CUsbPort(void);

public:
//	CWnd*	m_pOwner;
	HANDLE	m_DeviceHandle;
	HIDP_CAPS	Capabilities;

public:
	int		InitUsbPort(int CommType, char *pDesc);

	BOOL	USBSCSIRead(HANDLE hHandle, BYTE* pCDB, DWORD nCDBLen, BYTE* pData, DWORD nLength, DWORD nTimeOut);
	BOOL	USBSCSIWrite(HANDLE hHandle,BYTE* pCDB, DWORD nCDBLen, BYTE* pData, DWORD nLength, DWORD nTimeOut);

	BOOL	USBHidWrite(HANDLE hHandle,BYTE* pData, DWORD nLength, DWORD nTimeOut);
	BOOL	USBHidRead(HANDLE hHandle, BYTE* pData, DWORD nLength, DWORD nTimeOut);

	int		CloseUsbPort(void);

private:
	int OpenUsbMsc(char *pDesc);
	int OpenScsiDiskDevice(HDEVINFO IntDevInfo, DWORD Index, DWORD* pDeviceHandle, char *pDesc);
	ULONG GetMediaType(HANDLE hDevice, PUCHAR cdTypeString);
	int OpenUsbDriver(char *pDesc);
	HANDLE OpenOneDevice(HDEVINFO HardwareDeviceInfo, PSP_DEVICE_INTERFACE_DATA DeviceInfoData, char *pDesc);
};

extern CUsbPort UsbPort;

