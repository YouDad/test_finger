// UsbHid.cpp: implementation of the UsbHid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "UsbHidStr.h"
#include "UsbHid.h"

extern "C" {
#include "setupapi.h"
#include "hidsdi.h"
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		REPORT_COUNT	64		//端点长度
#define		USBD_VID        0x1183 
#define		USBD_PID        0x5711 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUsbHid::CUsbHid()
{
	hWriteHandle=hReadHandle=INVALID_HANDLE_VALUE;
}

CUsbHid::~CUsbHid()
{

}

bool CUsbHid::OpenHid(USHORT Vid, USHORT Pid)
{
	//用来保存设备是否已经找到
	BOOL DevFound=FALSE;
	//用来保存找到的设备路径
	CString DevPathName="";
	//定义一个GUID的结构体HidGuid来保存HID设备的接口类GUID。
	GUID HidGuid;
	//定义一个DEVINFO的句柄hDevInfoSet来保存获取到的设备信息集合句柄。
	HDEVINFO hDevInfoSet;
	//定义MemberIndex，表示当前搜索到第几个设备，0表示第一个设备。
	DWORD MemberIndex;
	//DevInterfaceData，用来保存设备的驱动接口信息
	SP_DEVICE_INTERFACE_DATA DevInterfaceData;
	//定义一个BOOL变量，保存函数调用是否返回成功
	BOOL Result;
	//定义一个RequiredSize的变量，用来接收需要保存详细信息的缓冲长度。
	DWORD RequiredSize;
	//定义一个指向设备详细信息的结构体指针。
	PSP_DEVICE_INTERFACE_DETAIL_DATA	pDevDetailData;
	//定义一个用来保存打开设备的句柄。
	HANDLE hDevHandle;
	//定义一个HIDD_ATTRIBUTES的结构体变量，保存设备的属性。
	HIDD_ATTRIBUTES DevAttributes;

	//对DevInterfaceData结构体的cbSize初始化为结构体大小
	DevInterfaceData.cbSize=sizeof(DevInterfaceData);
	//对DevAttributes结构体的Size初始化为结构体大小
	DevAttributes.Size=sizeof(DevAttributes);

	//调用HidD_GetHidGuid函数获取HID设备的GUID，并保存在HidGuid中。
	HidD_GetHidGuid(&HidGuid);

	//根据HidGuid来获取设备信息集合。其中Flags参数设置为
	//DIGCF_DEVICEINTERFACE|DIGCF_PRESENT，前者表示使用的GUID为
	//接口类GUID，后者表示只列举正在使用的设备，因为我们这里只
	//查找已经连接上的设备。返回的句柄保存在hDevinfo中。注意设备
	//信息集合在使用完毕后，要使用函数SetupDiDestroyDeviceInfoList
	//销毁，不然会造成内存泄漏。
	hDevInfoSet=SetupDiGetClassDevs(&HidGuid,
		                            NULL,
							       	NULL,
						            DIGCF_DEVICEINTERFACE|DIGCF_PRESENT);

	//MessageBox(NULL,"开始查找设备!",NULL,MB_OK);

	//然后对设备集合中每个设备进行列举，检查是否是我们要找的设备
	//当找到我们指定的设备，或者设备已经查找完毕时，就退出查找。
	//首先指向第一个设备，即将MemberIndex置为0。
	MemberIndex=0;
	while(1)
	{
		//调用SetupDiEnumDeviceInterfaces在设备信息集合中获取编号为
		//MemberIndex的设备信息。
		Result=SetupDiEnumDeviceInterfaces(hDevInfoSet,
											NULL,
											&HidGuid,
											MemberIndex,
											&DevInterfaceData);

		//如果获取信息失败，则说明设备已经查找完毕，退出循环。
		if(Result==FALSE) break;

		//将MemberIndex指向下一个设备
		MemberIndex++;

		//如果获取信息成功，则继续获取该设备的详细信息。在获取设备
		//详细信息时，需要先知道保存详细信息需要多大的缓冲区，这通过
		//第一次调用函数SetupDiGetDeviceInterfaceDetail来获取。这时
		//提供缓冲区和长度都为NULL的参数，并提供一个用来保存需要多大
		//缓冲区的变量RequiredSize。
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
												&DevInterfaceData,
												NULL,
												NULL,
												&RequiredSize,
												NULL);

		//然后，分配一个大小为RequiredSize缓冲区，用来保存设备详细信息。
		pDevDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(RequiredSize);
		if(pDevDetailData==NULL) //如果内存不足，则直接返回。
		{
			MessageBox(NULL,"内存不足!",NULL,MB_OK);
			SetupDiDestroyDeviceInfoList(hDevInfoSet);
			return false;
		}

		//并设置pDevDetailData的cbSize为结构体的大小（注意只是结构体大小，
		//不包括后面缓冲区）。
		pDevDetailData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		//然后再次调用SetupDiGetDeviceInterfaceDetail函数来获取设备的
		//详细信息。这次调用设置使用的缓冲区以及缓冲区大小。
		Result=SetupDiGetDeviceInterfaceDetail(hDevInfoSet,
												&DevInterfaceData,
												pDevDetailData,
												RequiredSize,
												NULL,
												NULL);

		//将设备路径复制出来，然后销毁刚刚申请的内存。
		DevPathName=pDevDetailData->DevicePath;
		free(pDevDetailData);

		//如果调用失败，则查找下一个设备。
		if(Result==FALSE) continue;

		//如果调用成功，则使用不带读写访问的CreateFile函数
		//来获取设备的属性，包括VID、PID、版本号等。
		//对于一些独占设备（例如USB键盘），使用读访问方式是无法打开的，
		//而使用不带读写访问的格式才可以打开这些设备，从而获取设备的属性。
		hDevHandle=CreateFile(DevPathName, 
								NULL,
								FILE_SHARE_READ|FILE_SHARE_WRITE, 
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

		//如果打开成功，则获取设备属性。
		if(hDevHandle!=INVALID_HANDLE_VALUE)
		{
			//获取设备的属性并保存在DevAttributes结构体中
			Result=HidD_GetAttributes(hDevHandle,
										&DevAttributes);

			//关闭刚刚打开的设备
			CloseHandle(hDevHandle);

			//获取失败，查找下一个
			if(Result==FALSE) continue;

			//如果获取成功，则将属性中的VID、PID以及设备版本号与我们需要的
			//进行比较，如果都一致的话，则说明它就是我们要找的设备。
			if(DevAttributes.VendorID==Vid) //如果VID相等
				if(DevAttributes.ProductID==Pid) //并且PID相等
					//if(DevAttributes.VersionNumber==MyPvn) //并且设备版本号相等
						{
							DevFound=TRUE; //设置设备已经找到
							//MessageBox(NULL,"设备已经找到!",NULL,MB_OK);

							//那么就是我们要找的设备，分别使用读写方式打开之，并保存其句柄
							//并且选择为异步访问方式。
							//读方式打开设备
							hReadHandle=CreateFile(DevPathName, 
													GENERIC_READ,
													FILE_SHARE_READ|FILE_SHARE_WRITE, 
													NULL,
													OPEN_EXISTING,
													FILE_ATTRIBUTE_NORMAL,//|FILE_FLAG_OVERLAPPED,
													NULL);

							if(hReadHandle!=INVALID_HANDLE_VALUE)
							{
								//MessageBox(NULL,"读访问打开设备成功!",NULL,MB_OK);
							}
							else 
								MessageBox(NULL,"读访问打开设备失败!",NULL,MB_OK);

							//写方式打开设备
							hWriteHandle=CreateFile(DevPathName, 
													GENERIC_WRITE,
													FILE_SHARE_READ|FILE_SHARE_WRITE, 
													NULL,
													OPEN_EXISTING,
													FILE_ATTRIBUTE_NORMAL,//|FILE_FLAG_OVERLAPPED,
													NULL);
							if(hWriteHandle!=INVALID_HANDLE_VALUE)
							{
								//MessageBox(NULL,"写访问打开设备成功!",NULL,MB_OK);
							}
							else 
								MessageBox(NULL,"写访问打开设备失败!",NULL,MB_OK);

							break;
						}
		}
		//如果打开失败，则查找下一个设备
		else continue;
	}

	//调用SetupDiDestroyDeviceInfoList函数销毁设备信息集合
	SetupDiDestroyDeviceInfoList(hDevInfoSet);

	//如果设备已经找到，那么应该使能各操作按钮，并同时禁止打开设备按钮
	if(DevFound)
	{
		return true;
	}
	else
	{
		return false;
	}	
}

void CUsbHid::CloseHid()
{
	//如果读数据的句柄不是无效句柄，则关闭之
	if(hReadHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hReadHandle);
		hReadHandle=INVALID_HANDLE_VALUE;
	}
	if(hWriteHandle!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(hWriteHandle);
		hWriteHandle=INVALID_HANDLE_VALUE;
	}
}

bool CUsbHid::WriteHid(BYTE *str, DWORD *len, DWORD *err)
{
	BOOL Result;
	UINT LastError;
	UINT i;
	DWORD strlen=0;
	UCHAR WriteReportBuffer[REPORT_COUNT+1]={0};

	//如果句柄无效，则说明打开设备失败
	if(hWriteHandle==INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,"无效的写报告句柄!",NULL,MB_OK);
		if(err!=NULL)
			*err=(DWORD)INVALID_HANDLE_VALUE;
		if(len!=NULL)
			*len=0;
		return false;
	}

	if(len==NULL)
		strlen=REPORT_COUNT+1;
	else
		strlen=*len;

	//设置要发送报告的数据
	WriteReportBuffer[0]=0x00;   //报告ID为0
	for(i=0;(i<REPORT_COUNT)&&(i<strlen);i++)
		WriteReportBuffer[i+1]=str[i];

	//调用WriteFile函数发送数据
	Result=WriteFile(hWriteHandle,
					WriteReportBuffer, 
					REPORT_COUNT+1,
					&strlen,
					NULL);
	if(len!=NULL)
		*len=strlen;

	//如果函数返回失败，则可能是真的失败，也可能是IO挂起了
	if(Result==FALSE)
	{
		//获取最后错误代码
		LastError=GetLastError();
		if(err!=NULL)
			*err=LastError;

		//看是否是真的IO挂起
		if((LastError==ERROR_IO_PENDING)||(LastError==ERROR_SUCCESS))
		{
			return true;
		}
		//否则，是函数调用时发生错误，显示错误代码
		else
		{
			CString cs;
			cs.Format("发送失败，错误代码：%d",LastError);
			MessageBox(NULL,cs,NULL,MB_OK);
			//如果最后错误为1，说明该设备不支持该函数。
			if(LastError==1)
			{
				MessageBox(NULL,"该设备不支持WriteFile函数。",NULL,MB_OK);
			}
			return false;
		}
	}
	//否则，函数返回成功
	else
	{
		return true;
	}	
}

bool CUsbHid::ReadHid(BYTE *str, DWORD *len, DWORD *err)
{
	BOOL Result;
	UINT i;
	DWORD strlen=0;
	UCHAR ReadReportBuffer[REPORT_COUNT+1]={0};

	if(hReadHandle==INVALID_HANDLE_VALUE) //如果读句柄无效
	{
		MessageBox(NULL,"无效的读报告句柄!",NULL,MB_OK);
		if(err!=NULL)
			*err=(DWORD)INVALID_HANDLE_VALUE;
		if(len!=NULL)
			*len=0;
		return false;
	}

	//则调用ReadFile函数请求9字节的报告数据
	Result=ReadFile(hReadHandle,
			ReadReportBuffer,
			REPORT_COUNT+1,
			&strlen,
			NULL);

	if(len!=NULL)
		*len=strlen;

	if(Result==FALSE)
	{
		if(err!=NULL)
			*err=GetLastError();
		if(len!=NULL)
			*len=0;
		return false;	
	}
	else
	{
		for(i=0;i<REPORT_COUNT;i++)
			str[i]=ReadReportBuffer[i+1];

		return true;
	}
}
