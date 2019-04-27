#pragma once
#include"stdafx.h"

class CCommunication{
private:
    static CSerial serial;
    static int id;
    static bool sendCommand_Default(int cmdCode,uint8_t* Data=0,uint16_t Len=0);
    static bool sendCommand_HangXin(int cmdCode,uint8_t* Data=0,uint16_t Len=0);
    static bool USB_Send(BYTE CmdBuf[],DWORD CmdLength);
    static bool USB_Receive(BYTE RspBuf[],DWORD RspLength);
public:
    static int getConnectId();
    static bool connect(int id,int baud);
    static bool disConnect();
    static bool sendCommand(int cmdCode,uint8_t* Data=0,uint16_t Len=0);
    static bool waitForPacket(int timeOutMs);
    //清洗串口包的函数,把包头和校验码洗掉,留下来数据放入packetData
    static void CCommunication::getDataFromPacket();
};

uint16_t GetCRC16(const void*pSource,uint16_t len);


///////////////////////命令码///////////////////////////////
#define	CMD_GET_TEST_IMAGE			0x031F			//获得背景图片
#define	CMD_DEVICE_RESET	    	0x0320	    	//系统复位
#define	CMD_DETECT_FINGER	    	0x0321	    	//探测指纹
#define	CMD_GET_RAW_IMAGE	    	0x0322	    	//采集原始图像
#define	CMD_GET_REDRESS_IMAGE		0x0323	    	//采集矫正图像
#define	CMD_UPLOAD_IMAGE	    	0x0324	    	//上传图像(包括原始图像和矫正图像)
#define	CMD_GEN_CHAR	    	   	0x0325	    	//生成模板
#define	CMD_MATCH_CHAR	    	   	0x0326	    	//比对模板
#define	CMD_STORE_CHAR	    	   	0x0327	    	//存储模板
#define	CMD_SEARCH_CHAR	    	   	0x0328	    	//搜索模板
#define	CMD_DELETE_CHAR	    	   	0x0329	    	//删除指定模板
#define	CMD_EMPTY_CHAR	    	   	0x032A	    	//清空模板库
#define	CMD_VERIFY_CHAR	    	   	0x032B	    	//单一比对模板
#define	CMD_GET_CHAR	    	   	0x032C			//获取模板
#define	CMD_PUT_CHAR	    	   	0x032D			//下载模板
#define	CMD_GET_MBINDEX	    	   	0x032E	    	//获取MBIndex
#define	CMD_READ_NOTE_BOOK	    	0x032F	    	//读记事本
#define	CMD_WRITE_NOTE_BOOK	    	0x0330	    	//写记事本
#define	CMD_READ_PAR_TABLE	    	0x0331	    	//读取参数表
#define	CMD_SET_BAUD_RATE	    	0x0332			//设置波特率
#define	CMD_SET_SECURLEVEL	    	0x0333	    	//设置指纹安全级别
#define	CMD_SET_CMOS_PARA	    	0x0334			//设置CMOS参数
#define	CMD_RESUME_FACTORY	    	0x0335	    	//恢复出厂设置
#define	CMD_MERGE_CHAR	    		0x0336			//合并特征
#define	CMD_SET_PSW	    			0x0337			//密码设置
#define	CMD_SET_ADDRESS	    		0x0338			//地址设置
#define	CMD_GET_RANDOM	    		0x0339			//获取随机数
#define	CMD_DOWNLOAD_IMAGE	    	0x0340	    	//下载指纹图像
#define	CMD_ERASE_PROGRAM	    	0x0341			//擦除程序标识
#define	CMD_STORE_CHAR_DIRECT	   	0x0342	    	//直接存储指纹模板
#define	CMD_READ_CHAR_DIRECT	   	0x0343	    	//直接根据地址读取指纹模板
#define	CMD_GET_FIRSTVALID_ADD		0x0344	    	//获取第一个空闲的指纹索引
#define	CMD_CHIP_ERASE	    		0x0380			//整片擦除FLASH
#define CMD_FINGER_DETECT			0X0400			//手指检测
#define	CMD_Sleep_MODE				0X0401			//sleep模式
#define CMD_SleepToIdle				0x0402
#define CMD_AdjustImage				0x0403
//返回码		    		    	
#define	CMD_RT_OK	    					0x0000	    	//命令执行完毕或OK
#define	CMD_RT_PACKGE_ERR	    			0x0001	    	//数据包接收错误
#define	CMD_RT_DEVICE_ADDRESS_ERR			0x0002	    	//设备地址错误
#define	CMD_RT_COM_PASSWORD_ERR	  			0x0003	    	//通信密码错误
#define	CMD_RT_NO_FINGER	    			0x0004	    	//传感器上没有手指
#define	CMD_RT_GET_IMAGE_FAIL	    		0x0005	    	//从传感器上获取图像失败
#define	CMD_RT_GEN_CHAR_ERR	    			0x0006	    	//生成特征失败
#define	CMD_RT_FINGER_MATCH_ERR	  			0x0007	    	//指纹不匹配
#define	CMD_RT_FINGER_SEARCH_FAIL			0x0008	    	//没搜索到指纹
#define	CMD_RT_MERGE_TEMPLET_FAIL			0x0009	    	//特征合并失败
#define	CMD_RT_ADDRESS_OVERFLOW	  			0x000A	    	//将模板存库时地址序号超出指纹库范围
#define	CMD_RT_READ_TEMPLET_ERR	  			0x000B	    	//从指纹库读模板出错
#define	CMD_RT_UP_TEMPLET_ERR	    		0x000C	    	//上传特征失败
#define	CMD_RT_UP_IMAGE_FAIL	    		0x000D	    	//上传图像失败
#define	CMD_RT_DELETE_TEMPLET_ERR			0x000E	    	//删除模板失败
#define	CMD_RT_CLEAR_TEMPLET_LIB_ERR		0x000F	    	//清空指纹库失败
#define	CMD_RT_FINGER_NOT_MOVE	    		0x0010	    	//残留指纹或传感器窗口的按指长时间未移开
#define	CMD_RT_NO_TEMPLET_IN_ADDRESS		0x0011	    	//指定位置没有有效模板
#define	CMD_RT_CHAR_REPEAT	    			0x0012	    	//指纹重复，需要注册的指纹已经在FLASH中注册
#define	CMD_RT_MB_NOT_EXIST_IN_ADDRESS		0x0013	    	//该地址中不存在指纹模板
#define	CMD_RT_GET_MBINDEX_OVERFLOW			0x0014	    	//获取模板索引长度溢出
#define	CMD_RT_SET_BAUD_RATE_FAIL	    	0x0015	    	//设置波特率失败
#define	CMD_RT_ERASE_FLAG_FAIL	    		0x0016	    	//擦除程序标志失败
#define	CMD_RT_SYSTEM_RESET_FAIL	    	0x0017	    	//系统复位失败
#define	CMD_RT_OPERATION_FLASH_ERR			0x0018	    	//操作FLASH出错
#define	CMD_RT_NOTE_BOOK_ADDRESS_OVERFLOW	0x0019			//记事本地址溢出
#define	CMD_RT_PARA_ERR	    				0x001A	    	//设置参数错误
#define	CMD_RT_NO_CMD	    				0x001B	    	//命令不存在


////HangXin
//enum HangXinCmd{
//    USR_CMD_GET_INFO=1,
//    USR_CMD_GET_DEVICESN,
//    USR_CMD_SET_DEVICESN,
//    USR_CMD_GET_SESSION,
//    USR_CMD_SET_SESSION,
//    USR_CMD_GET_SENSOR_INFO,
//    USR_CMD_CONFIG_SENSOR,
//    USR_CMD_GRAB,
//    USR_CMD_GRAB_NO_CHECK,
//    USR_CMD_GRAB_WAIT,
//    USR_CMD_GENERATE,
//    USR_CMD_MERGE,
//    USR_CMD_STORE,
//    USR_CMD_SEARCH,
//    USR_CMD_MATCH,
//    USR_CMD_UP_IMG,
//    USR_CMD_DOWN_IMG,
//    USR_CMD_LOAD_CHAR,
//    USR_CMD_UP_CHAR,
//    USR_CMD_DOWN_CHAR,
//    USR_CMD_LIST,
//    USR_CMD_GET_EMPTY_ID,
//    USR_CMD_CHECK_ID,
//    USR_CMD_DELETE_ID,
//    USR_CMD_REMOVE_ALL,
//    USR_CMD_SET_LED,
//    USR_CMD_ENTER_IAP,
//    USR_CMD_CANCEL,
//    USR_CMD_WIRTE_PRODUCT_SESSION,
//    USR_CMD_SLEEP,
//    USR_CMD_DEV_BOOTLOADER,
//    USR_CMD_UP_IMG_EX=0x22
//};

#define COMM_USB_MASS	0
#define COMM_UART		1
#define COMM_USB_HID	2
#define COMM_USB_DRIVER	3

#define SENSOR_TYPE_NONE		0
#define SENSOR_TYPE_YW			1
#define SENSOR_TYPE_NB			2
#define SENSOR_TYPE_FPC1011F	3