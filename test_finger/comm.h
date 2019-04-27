#ifndef _COMM_H
#define _COMM_H


#define COMM_USB_MASS	0
#define COMM_UART		1
#define COMM_USB_HID	2
#define COMM_USB_DRIVER	3

#define SENSOR_TYPE_NONE		0
#define SENSOR_TYPE_YW			1
#define SENSOR_TYPE_NB			2
#define SENSOR_TYPE_FPC1011F	3

extern int CommType;

//typedef struct _SessionCfg
//{
//	DWORD BaudRate;
//	BYTE  SecurityLevel;
//	BYTE  AutoLearn;
//	BYTE  DupCheck;
//	BYTE  FingerImg_Var_Thr;
//	BYTE  FingerImg_Percent_Thr;
//}STRUCT_SESSION_CFG; 
//
//typedef struct{
//	bool connected;					//True if module has been powered up and successful SPI handshake (NEXT_Connect_Diagnostic)
//	bool boot_complete;
//	bool stopped;					//True if module has been stopped with NEXT_Module_Stop
//	bool column_scan;				//True if module scans in column direction
//	BYTE firmware_ver[3];		//Firmware version on module
//	BYTE serial_no[12];			//Serial number of MCU on module
//	//	NEXT_SENSOR_IMAGE_t image;		//Module image
//}NEXT_MODULE_t;
//
//typedef struct YW_MODULE
//{
//	unsigned char reg[32];
//	unsigned char work_mode;
//
//}YW_MODULE_t;
//
//typedef struct _FPC1011F_MODULE
//{
//	BYTE reg[16];
//	BYTE drivc;
//	BYTE adcref;
//}FPC1011F_MODULE_t;
//
//typedef struct SensorInfo
//{
//	BYTE SensorType;
//	union
//	{
//		NEXT_MODULE_t NB_Info;
//		YW_MODULE_t	  YW_Info;	
//		FPC1011F_MODULE_t FPC1011F_Info; 
//	}Info;
//}SensorInfo_t;

typedef unsigned long       DWORD;

typedef unsigned char       BYTE;

int Comm_SendReceive(BYTE CmdBuf[],DWORD CmdLength,BYTE RspBuf[],DWORD RspLength,DWORD nTimeout);

#endif