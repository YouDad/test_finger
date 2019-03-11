#if !defined(AFX_COMMAND_H__140A1824_D14D_42C0_A498_48CE7FF70937__INCLUDED_)
#define AFX_COMMAND_H__140A1824_D14D_42C0_A498_48CE7FF70937__INCLUDED_

#include "Serial.h"

//////////////////////////////	DEFINE	////////////////////////////////////////////
#define CMD_PREFIX_CODE							0xAA55
#define RCM_PREFIX_CODE							0x55AA
#define CMD_DATA_PREFIX_CODE					0xA55A
#define RCM_DATA_PREFIX_CODE					0x5AA5

#define CMD_VERIFY_CODE							0x0101
#define CMD_IDENTIFY_CODE						0x0102
#define CMD_ENROLL_CODE							0x0103
#define CMD_ENROLL_ONETIME_CODE					0x0104
#define CMD_CLEAR_TEMPLATE_CODE					0x0105
#define CMD_CLEAR_ALLTEMPLATE_CODE				0x0106
#define CMD_GET_EMPTY_ID_CODE					0x0107
#define CMD_GET_BROKEN_TEMPLATE_CODE			0x0109
#define CMD_READ_TEMPLATE_CODE					0x010A
#define CMD_WRITE_TEMPLATE_CODE					0x010B
#define CMD_GET_FW_VERSION_CODE					0x0112
#define CMD_FINGER_DETECT_CODE					0x0113
#define CMD_FEATURE_OF_CAPTURED_FP_CODE			0x011A
#define CMD_IDENTIFY_TEMPLATE_WITH_FP_CODE		0x011C
#define CMD_SLED_CTRL_CODE						0x0124
#define CMD_IDENTIFY_FREE_CODE					0x0125
#define CMD_SET_DEVPASS_CODE					0x0126
#define CMD_VERIFY_DEVPASS_CODE					0x0127
#define CMD_GET_ENROLL_COUNT_CODE				0x0128
#define CMD_CHANGE_TEMPLATE_CODE				0x0129
#define CMD_UP_IMAGE_CODE						0x012C
#define	CMD_VERIFY_WITH_DOWN_TMPL_CODE			0x012D
#define	CMD_IDENTIFY_WITH_DOWN_TMPL_CODE		0x012E
#define CMD_FP_CANCEL_CODE						0x0130
#define	CMD_ADJUST_SENSOR_CODE					0x0137
#define CMD_IDENTIFY_WITH_IMAGE_CODE			0x0138
#define CMD_VERIFY_WITH_IMAGE_CODE				0x0139
#define	CMD_SET_PARAMETER_CODE					0x013A
#define	CMD_EXIT_DEVPASS_CODE					0x013B
#define CMD_SET_COMMNAD_VALID_FLAG_CODE			0x013C
#define CMD_GET_COMMNAD_VALID_FLAG_CODE			0x013D
#define CMD_TEST_CONNECTION_CODE				0x0150
#define RCM_INCORRECT_COMMAND_CODE				0x0160
#define	CMD_ENTER_ISPMODE_CODE					0x0171

/***************************************************************************/

#define	ERR_FAIL					1
#define	ERR_SUCCESS					0
#define	ERR_CONTINUE				2

#define	ERR_VERIFY					0x11
#define	ERR_IDENTIFY				0x12
#define	ERR_TMPL_EMPTY				0x13
#define	ERR_TMPL_NOT_EMPTY			0x14
#define	ERR_ALL_TMPL_EMPTY			0x15
#define	ERR_EMPTY_ID_NOEXIST		0x16
#define	ERR_BROKEN_ID_NOEXIST		0x17
#define	ERR_INVALID_TMPL_DATA		0x18
#define	ERR_DUPLICATION_ID			0x19
#define	ERR_TOO_FAST				0x20
#define	ERR_BAD_QUALITY				0x21
#define	ERR_SMALL_LINES				0x22
#define	ERR_TIME_OUT				0x23
#define	ERR_NOT_AUTHORIZED			0x24
#define	ERR_GENERALIZE				0x30
#define	ERR_COM_TIMEOUT				0x40
#define	ERR_FP_CANCEL				0x41
#define	ERR_INTERNAL				0x50
#define	ERR_MEMORY					0x51
#define	ERR_EXCEPTION				0x52
#define	ERR_INVALID_TMPL_NO			0x60
#define	ERR_INVALID_PARAM			0x70
#define	ERR_NO_RELEASE				0x71
#define	ERR_INVALID_OPERATION_MODE	0x72
#define ERR_NOT_SET_PWD				0x74
#define	ERR_FP_NOT_DETECTED			0x75
#define	ERR_ADJUST_SENSOR			0x76

#define	GD_NEED_FIRST_SWEEP			0xFFF1
#define	GD_NEED_SECOND_SWEEP		0xFFF2
#define	GD_NEED_THIRD_SWEEP			0xFFF3
#define	GD_NEED_RELEASE_FINGER		0xFFF4

#define	GD_TEMPLATE_NOT_EMPTY		0x01
#define	GD_TEMPLATE_EMPTY			0x00

#define	GD_DETECT_FINGER			0x01
#define	GD_NO_DETECT_FINGER			0x00

#define	GD_DOWNLOAD_SUCCESS			0xA1

/***************************************************************************/
#define	MAX_DATA_LEN					26627//512

#define	MIN_DEV_ID						1
#define MAX_DEV_ID						255
#define	DEFAULT_DEV_ID					1
#define	GD_MAX_DEVICE_ID_LEN			1

#define	GD_LOW_SEC_LEVEL				1
#define	GD_HIGH_SEC_LEVEL				5

#define	GD_MAX_FP_TIME_OUT				60
#define	GD_DEFAUT_FP_TIME_OUT			5
#define	GD_MIN_FP_TIME_OUT				0

/*******************************************************************************
* Define Parameter Index
********************************************************************************/
#define	DP_DEVICE_ID					0
#define	DP_SECURITY_LEVEL				1
#define	DP_DUP_CHECK					2
#define	DP_BAUDRATE						3
#define	DP_AUTO_LEARN					4
#define	DP_FP_TIMEOUT					5
#define	DP_OPERATION_MODE				6

#define GD_TEMPLATE_SIZE				(496)
#define GD_RECORD_SIZE					(GD_TEMPLATE_SIZE + 2)	// CkeckSum len = 2

#define GD_IMAGE_RECEIVE_UINT			(498)
#define	GD_MAX_RECORD_COUNT				5000

#define	COMM_TIMEOUT					15000
//////////////////////////////	STRUCT	////////////////////////////////////////////
#pragma pack(1)

typedef struct _ST_COM_PACKET_
{
	WORD	wPrefix;
	WORD	wCMD_RCM;
	WORD	wDataLen;
	union	
	{
		BYTE	tbCMDData[16];
		struct	 
		{
			WORD	wRet;
			BYTE	tbRcmData[14];
		} stRcmData;
	};
} ST_COM_PACKET, *PST_COM_PACKET;

typedef struct _ST_COMMAND_
{
	char	szCommandName[64];
	WORD	wCode;
} ST_COMMAND, *PST_COMMAND;

#pragma pack()

#define	COMMAND_DATALEN				(g_pPacketBuffer->wDataLen)
#define	COMMAND_DATA1				(*(WORD*)g_pPacketBuffer->tbCMDData)
#define	COMMAND_DATA2				(*(WORD*)&g_pPacketBuffer->tbCMDData[2])
#define	COMMAND_CHECK				*(WORD*)&g_Packet[CMD_PACKET_LEN]
#define	RESPONSE_DATA1				(*(WORD*)g_pPacketBuffer->stRcmData.tbRcmData)
#define	RESPONSE_DATA2				(*(WORD*)(g_pPacketBuffer->stRcmData.tbRcmData+2))
#define	RESPONSE_RET				(g_pPacketBuffer->stRcmData.wRet)

#define	CMD_PACKET_LEN				(sizeof(ST_COM_PACKET))
#define	DATA_PACKET_LEN				(g_pPacketBuffer->wDataLen + 6)
/////////////////////////////	Value	/////////////////////////////////////////////
//extern CSerial			g_Serial;
// extern BYTE				g_Packet[MAX_DATA_LEN + 10];
extern PST_COM_PACKET	g_pPacketBuffer;
extern ST_COMMAND		g_Commands[];

/////////////////////////////	Function	/////////////////////////////////////////////
#define SEND_COMMAND(cmd, ret)	\
	if ( m_nConnectionMode == 0 )	\
	ret = SendCommand(cmd);		\
	else							\
ret = USB_SendPacket(m_UsbHandle, cmd)

#define SEND_DATAPACKET(cmd, ret)	\
	if ( m_nConnectionMode == 0 )	\
	ret = SendDataPacket(cmd);	\
	else							\
ret = USB_SendDataPacket(m_UsbHandle, cmd)

#define RECEIVE_DATAPACKET(cmd, len, ret)	\
	if ( m_nConnectionMode == 0 )			\
	ret = ReceiveDataPacket(cmd, len);	\
	else									\
		ret = USB_ReceiveDataPacket(m_UsbHandle, cmd, len)

/////////////////////////////	Function	/////////////////////////////////////////////
WORD GetCheckSum(bool bCmdData);
WORD AddCheckSum( bool bCmdData );
bool CheckReceive( WORD p_Prefix, WORD p_CMD );
void InitPacket(WORD p_wCMD, bool p_bCmdData);
bool SendCommand(WORD p_wCMD);
bool ReceiveAck(WORD p_wCMD, bool p_bCmdData);

bool SendData(WORD	p_wCMD, int p_nDataLen, PBYTE p_pData );
bool SendDataPacket(WORD p_wCMD);
bool ReceiveData(WORD	p_wCMD, int p_nDataLen, PBYTE p_pData);
bool ReceiveDataPacket(WORD p_wCMD, WORD p_wDataLen);
bool ReadDataN(BYTE* p_pData, int p_nLen, DWORD p_dwTimeOut);

CString GetErrorMsg(WORD p_wErrorCode);

#endif // !defined(AFX_COMMAND_H__140A1824_D14D_42C0_A498_48CE7FF70937__INCLUDED_)
