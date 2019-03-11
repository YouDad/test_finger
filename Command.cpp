/********************************************************************
	created:	2009/06/08
	created:	8:6:2009   22:13
	filename: 	f:\Work\STM32\STO20\STO20_Host\Command.cpp
	file path:	f:\Work\STM32\STO20\STO20_Host
	file base:	Command
	file ext:	cpp
	author:		KYM
	
	purpose:	통신파케트 관련 구조체 및 함수들을 정의한다.
*********************************************************************/

#include "stdafx.h"
#include "Command.h"
#include "HostMainDlg.h"

//CSerial			g_Serial;
//BYTE			g_Packet[MAX_DATA_LEN + 10];
PST_COM_PACKET	g_pPacketBuffer = (PST_COM_PACKET)g_Packet;

extern CHostMainDlg*	g_pMainDlg;

ST_COMMAND	g_Commands[] = 
{
	{"Enroll",											CMD_ENROLL_CODE},
	{"Verify",											CMD_VERIFY_CODE},
	{"Identify",										CMD_IDENTIFY_CODE},
	{"Identify Free",									CMD_IDENTIFY_FREE_CODE},
	{"Enroll One Time",									CMD_ENROLL_ONETIME_CODE},
	{"Change Template",									CMD_CHANGE_TEMPLATE_CODE},
	{"Clear Template",									CMD_CLEAR_TEMPLATE_CODE},
	{"Clear All Template",								CMD_CLEAR_ALLTEMPLATE_CODE},
	{"Get Empty ID",									CMD_GET_EMPTY_ID_CODE},
	{"Get Broken Template",								CMD_GET_BROKEN_TEMPLATE_CODE},
	{"Read Template",									CMD_READ_TEMPLATE_CODE},
	{"Write Template",									CMD_WRITE_TEMPLATE_CODE},
	{"Set Parameter",									CMD_SET_PARAMETER_CODE},
	{"Get F/W Version",									CMD_GET_FW_VERSION_CODE},
	{"Finger Detect",									CMD_FINGER_DETECT_CODE},
	{"Get Feature Data of Captured FP",					CMD_FEATURE_OF_CAPTURED_FP_CODE},
	{"Identify Downloaded Feature with Captured FP",	CMD_IDENTIFY_TEMPLATE_WITH_FP_CODE},
	{"Set Device Password",								CMD_SET_DEVPASS_CODE},
	{"Verify Device Password",							CMD_VERIFY_DEVPASS_CODE},
	{"Exit With Password",								CMD_EXIT_DEVPASS_CODE},
	{"Get Enroll Count",								CMD_GET_ENROLL_COUNT_CODE},
	{"Up Image",										CMD_UP_IMAGE_CODE},
	{"Identify with Downloaded Image",					CMD_IDENTIFY_WITH_IMAGE_CODE},
	{"Verify with Downloaded Image",					CMD_VERIFY_WITH_IMAGE_CODE},
	{"Identify with Downloaded Featrue",				CMD_IDENTIFY_WITH_DOWN_TMPL_CODE},
	{"Verify with Downloaded Feature",					CMD_VERIFY_WITH_DOWN_TMPL_CODE},
	{"Adjust Sensor",									CMD_ADJUST_SENSOR_CODE},
	{"Set Command Valid Flag",							CMD_SET_COMMNAD_VALID_FLAG_CODE},
	{"Get Command Valid Flag",							CMD_GET_COMMNAD_VALID_FLAG_CODE},
	{"Enter ISP Mode",									CMD_ENTER_ISPMODE_CODE},
	{0,0}
};
/***************************************************************************/
/***************************************************************************/
WORD GetCheckSum(bool bCmdData)
{
	WORD	w_Ret = 0;
	for ( WORD i = 0; i < (bCmdData?CMD_PACKET_LEN:DATA_PACKET_LEN); i++ )
		w_Ret += (WORD)g_Packet[i];
	return w_Ret;
}
/***************************************************************************/
/***************************************************************************/
WORD AddCheckSum( bool bCmdData )
{
	WORD	w_Ret = 0;
	WORD	w_Len = bCmdData?CMD_PACKET_LEN:DATA_PACKET_LEN;
	for ( int i = 0; i < w_Len; i++ )
		w_Ret += (WORD)g_Packet[i];
	*(WORD*)&g_Packet[w_Len] = w_Ret;
	return w_Ret;
}
/***************************************************************************/
/***************************************************************************/
bool CheckReceive( WORD p_Prefix, WORD p_CMD )
{
	WORD	w_wCheckSum;
//	if ( g_pPacketBuffer->wCMD_RCM != CMD_FP_CANCEL_CODE )
	{
		if ( p_Prefix != g_pPacketBuffer->wPrefix ||
			p_CMD != g_pPacketBuffer->wCMD_RCM )
			return false;
	}

	WORD	w_Len = (p_Prefix==RCM_PREFIX_CODE)?CMD_PACKET_LEN:DATA_PACKET_LEN;
	w_wCheckSum = *(WORD*)&g_Packet[w_Len];
	if ( w_wCheckSum != GetCheckSum(p_Prefix==RCM_PREFIX_CODE) )
		return false;
	return true;
}
/***************************************************************************/
/***************************************************************************/
void InitPacket(WORD p_wCMD, bool p_bCmdData)
{
	memset( g_Packet, 0, sizeof( g_Packet ));
	g_pPacketBuffer->wPrefix = p_bCmdData?CMD_PREFIX_CODE:CMD_DATA_PREFIX_CODE;
	g_pPacketBuffer->wCMD_RCM = p_wCMD;
	return;
}
/***************************************************************************/
/***************************************************************************/
bool SendCommand(WORD p_wCMD)
{
	DWORD	w_nSendCnt = 0, w_nTimeOut;
	LONG	w_nResult = 0;

	if (p_wCMD == CMD_TEST_CONNECTION_CODE)
		w_nTimeOut = 1500;
	else
		w_nTimeOut = COMM_TIMEOUT;

	w_nResult = g_Serial.Write(g_Packet, CMD_PACKET_LEN+2 , &w_nSendCnt, NULL, w_nTimeOut);

	if(ERROR_SUCCESS != w_nResult){
		::MessageBox(NULL, "Fail in sending a command packet !", "Communication Error", MB_ICONWARNING );
		return false;
	}

	return ReceiveAck(p_wCMD, true);
}
/***************************************************************************/
/***************************************************************************/
bool ReceiveAck(WORD	p_wCMD, bool p_bCmdData)
{
	DWORD	w_nAckCnt = 0;
	LONG	w_nResult = 0;
	DWORD	w_dwTimeOut = COMM_TIMEOUT;

	if ( p_wCMD == CMD_VERIFY_CODE ||
		 p_wCMD == CMD_IDENTIFY_CODE ||
		 p_wCMD == CMD_IDENTIFY_FREE_CODE ||
		 p_wCMD == CMD_ENROLL_CODE ||
		 p_wCMD == CMD_ENROLL_ONETIME_CODE ||
		 p_wCMD == CMD_CHANGE_TEMPLATE_CODE ||
		 p_wCMD == CMD_FEATURE_OF_CAPTURED_FP_CODE ||
		 p_wCMD == CMD_IDENTIFY_TEMPLATE_WITH_FP_CODE ||
		 p_wCMD == CMD_FINGER_DETECT_CODE)
		 //w_dwTimeOut = (g_pMainDlg->m_nTimeOut == 0) ? INFINITE : (g_pMainDlg->m_nTimeOut+1)*(1000);
		 w_dwTimeOut = GD_MAX_FP_TIME_OUT*1000;
	else if (p_wCMD == CMD_TEST_CONNECTION_CODE)
		w_dwTimeOut = 1500;

	w_nResult = g_Serial.Read(g_Packet, CMD_PACKET_LEN+2, &w_nAckCnt, NULL, w_dwTimeOut);
	if ( ERROR_SUCCESS != w_nResult ){
	::MessageBox(NULL, "Please check connection with the device !", "Communication Error", MB_ICONWARNING );
		return false;
	}
	
	return CheckReceive( p_bCmdData?RCM_PREFIX_CODE:RCM_DATA_PREFIX_CODE, p_wCMD );
}
/***************************************************************************/
/***************************************************************************/
bool SendData(WORD	p_wCMD, int p_nDataLen, PBYTE p_pData )
{
	int		w_nSendedCnt;
	int		w_wPacketDataLen = 0;
	LONG	w_nResult = 0;

	for ( w_nSendedCnt = 0; w_nSendedCnt < p_nDataLen; w_nSendedCnt += w_wPacketDataLen)
	{
		w_wPacketDataLen = p_nDataLen - w_nSendedCnt;
		if ( w_wPacketDataLen > MAX_DATA_LEN ) w_wPacketDataLen = MAX_DATA_LEN;

		InitPacket( p_wCMD, false );
		g_pPacketBuffer->wDataLen = (WORD)w_wPacketDataLen;
		memcpy( g_pPacketBuffer->tbCMDData, p_pData+w_nSendedCnt, w_wPacketDataLen );
		AddCheckSum( false );

		if ( SendDataPacket(p_wCMD) == false )
			return false;
	}
	return true;
}
/***************************************************************************/
/***************************************************************************/
bool ReceiveDataAck(WORD	p_wCMD)
{
	DWORD	w_nAckCnt = 0;
	LONG	w_nResult = 0;
	DWORD	w_dwTimeOut = COMM_TIMEOUT;

	if (p_wCMD == CMD_VERIFY_CODE ||
		p_wCMD == CMD_IDENTIFY_CODE ||
		p_wCMD == CMD_IDENTIFY_FREE_CODE ||
		p_wCMD == CMD_ENROLL_CODE ||
		p_wCMD == CMD_ENROLL_ONETIME_CODE ||
		p_wCMD == CMD_CHANGE_TEMPLATE_CODE ||
		p_wCMD == CMD_FEATURE_OF_CAPTURED_FP_CODE ||
		p_wCMD == CMD_IDENTIFY_TEMPLATE_WITH_FP_CODE )
			//w_dwTimeOut = (g_pMainDlg->m_nTimeOut == 0) ? INFINITE : (g_pMainDlg->m_nTimeOut+1)*(1000);
			w_dwTimeOut = GD_MAX_FP_TIME_OUT*1000;

// 	w_nResult = g_Serial.Read(g_Packet, 6, &w_nAckCnt, NULL, w_dwTimeOut);	
// 	if ( ERROR_SUCCESS != w_nResult ){
// 		::MessageBox(NULL, "Please check connection with the device !", "Communication Error", MB_ICONWARNING );
// 		return false;
// 	}
// 	w_nResult = g_Serial.Read(g_Packet+6, COMMAND_DATALEN+2, &w_nAckCnt, NULL, w_dwTimeOut);	

	if (!ReadDataN(g_Packet, 6, w_dwTimeOut))
		return false;

	if (!ReadDataN(g_Packet+6, COMMAND_DATALEN+2, w_dwTimeOut))
		return false;

	return CheckReceive( RCM_DATA_PREFIX_CODE, p_wCMD );
}
/***************************************************************************/
/***************************************************************************/
bool SendDataPacket(WORD	p_wCMD)
{
	DWORD	w_nSendCnt = 0;
	LONG	w_nResult = 0;

	w_nResult = g_Serial.Write(g_Packet, DATA_PACKET_LEN+2 , &w_nSendCnt, NULL, COMM_TIMEOUT);
	if(ERROR_SUCCESS != w_nResult){
		::MessageBox(NULL, "Fail in sending a command packet !", "Communication Error", MB_ICONWARNING );
		return false;
	}
//	return ReceiveAck(p_wCMD, false);
	return ReceiveDataAck(p_wCMD);
}
/***************************************************************************/
/***************************************************************************/
bool ReceiveData(WORD	p_wCMD, int p_nDataLen, PBYTE p_pData)
{
	int		w_nReceivedCnt;
	int		w_wPacketDataLen = 0;
	LONG	w_nResult = 0;

	for ( w_nReceivedCnt = 0; w_nReceivedCnt < p_nDataLen; w_nReceivedCnt += w_wPacketDataLen)
	{
		w_wPacketDataLen = p_nDataLen - w_nReceivedCnt;
		if ( w_wPacketDataLen > MAX_DATA_LEN ) w_wPacketDataLen = MAX_DATA_LEN;
		if ( ReceiveDataPacket(p_wCMD, w_wPacketDataLen) == false )
			return false;
		memcpy( p_pData+w_nReceivedCnt, g_pPacketBuffer->stRcmData.tbRcmData, DATA_PACKET_LEN-2 );
	}
	return true;
}
/***************************************************************************/
/***************************************************************************/
bool ReceiveDataPacket(WORD	p_wCMD, WORD p_wDataLen)
{
/*	DWORD	w_nDataCnt = 0;
	LONG	w_nResult = 0;

	w_nResult = g_Serial.Read(g_Packet, p_wDataLen+10, &w_nDataCnt, NULL, COMM_TIMEOUT);
	if ( ERROR_SUCCESS != w_nResult ){
		::MessageBox(NULL, "Please check connection with the device !", "Communication Error", MB_ICONWARNING );
		return false;
	}

	return CheckReceive( RCM_DATA_PREFIX_CODE, p_wCMD );
	*/
	return ReceiveDataAck(p_wCMD);
}
/***************************************************************************/

/***************************************************************************/
CString GetErrorMsg(WORD p_wErrorCode)
{
	CString w_ErrMsg;
	switch(LOBYTE(p_wErrorCode))
	{
	case ERR_VERIFY:
		w_ErrMsg = "Verify NG";
		break;
	case ERR_IDENTIFY:
		w_ErrMsg = "Identify NG";
		break;
	case ERR_EMPTY_ID_NOEXIST:
		w_ErrMsg = "Empty Template no Exist";
		break;
	case ERR_BROKEN_ID_NOEXIST:
		w_ErrMsg = "Broken Template no Exist";
		break;
	case ERR_TMPL_NOT_EMPTY:
		w_ErrMsg = "Template of this ID Already Exist";
		break;
	case ERR_TMPL_EMPTY:
		w_ErrMsg = "This Template is Already Empty";
		break;
	case ERR_INVALID_TMPL_NO:
		w_ErrMsg = "Invalid Template No";
		break;
	case ERR_ALL_TMPL_EMPTY:
		w_ErrMsg = "All Templates are Empty";
		break;
	case ERR_INVALID_TMPL_DATA:
		w_ErrMsg = "Invalid Template Data";
		break;
	case ERR_DUPLICATION_ID:
//		w_ErrMsg.Format("Duplicated ID : %d.", HIBYTE(p_wErrorCode));
		w_ErrMsg = "Duplicated ID : ";
		break;
	case ERR_BAD_QUALITY:
		w_ErrMsg = "Bad Quality Image";
		break;
	case ERR_SMALL_LINES:
		w_ErrMsg = "Small line Image";
		break;
	case ERR_TOO_FAST:
		w_ErrMsg = "Too fast swiping";
		break;
	case ERR_TIME_OUT:
		w_ErrMsg = "Time Out";
		break;
	case ERR_GENERALIZE:
		w_ErrMsg = "Fail to Generalize";
		break;
	case ERR_NOT_AUTHORIZED:
		w_ErrMsg = "Device not authorized.";
		break;
	case ERR_EXCEPTION:
		w_ErrMsg = "Exception Error ";
		break;
	case ERR_MEMORY:
		w_ErrMsg = "Memory Error ";
		break;
	case ERR_INVALID_PARAM:
		w_ErrMsg = "Invalid Parameter";
		break;
	case ERR_NO_RELEASE:
		w_ErrMsg = "No Release Finger Fail";
		break;
	case ERR_INTERNAL:
		w_ErrMsg = "Internal Error.";
		break;
	case ERR_FP_CANCEL:
		w_ErrMsg = "Canceled.";
		break;
	case ERR_INVALID_OPERATION_MODE:
		w_ErrMsg = "Invalid Operation Mode";
		break;
	case ERR_NOT_SET_PWD:
		w_ErrMsg = "Password was not set.";
		break;
	case ERR_FP_NOT_DETECTED:
		w_ErrMsg = "Finger is not detected.";
		break;
	case ERR_ADJUST_SENSOR:
		w_ErrMsg = "Failed to adjust sensor.";
		break;
	default:
		w_ErrMsg = "Fail";
		break;

	}
	return w_ErrMsg;
}

