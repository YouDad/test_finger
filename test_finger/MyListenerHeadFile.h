#pragma once

class GET_RAW_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class GET_TEST_IMAGE_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class READ_REGISTER_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_AdvDbg_AdjImg_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_Info_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_MeasureTimeStart_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

class CmdCodeLOG_MeasureTimeEnd_Listener:public ICommListener{
public:
    void listen(DataPacket response);
};

#pragma region SCC_DLC_ILC_BCL宏定义

/*
OS:前面是不用宏的定义,很占地儿,看起来很费劲
    给一个协议添加一个监听器有如下步骤:
    1.SCC,定义一个命令            本文件下面的枚举类型定义里
    2.DLC,定义一个监听器          本文件枚举类型定义前
    3.ILC,实现这个监听器          协议名的C++文件里
    4.BCL,把命令绑到这个监听器上   监听器广播的C++文件里监听器广播构造函数

    可能还需要设置对应命令转化函数,有如下步骤:
    1.在此文件顶部添加软件内部指令
    2.在对应协议Converter的C++文件里的转化函数增加一个case块

    注:监听器广播是ListenerBoardcast
*/

//___SCN===Structure Class Name,快速拼凑类名的辅助宏
#define ___SCN(Protocol,Message) \
CmdCode##Protocol##_##Message##_Listener

//__SCC===Structure Command Code,快速拼凑命令的宏
#define __SCC(Protocol,Message) \
CmdCode##Protocol##_##Message

//__DLC===Define Listener Class,快速定义监听器类的宏
#define __DLC(Protocol,Message) \
class ___SCN(Protocol,Message):public ICommListener{\
public:\
    void listen(DataPacket response);\
}

//__BCL===Binding Command into Listener,快速把命令绑到监听器的宏
#define __BCL(Protocol,Message) \
attach(__SCC(Protocol,Message),new ___SCN(Protocol,Message)())

//__ILC===Implement Listener Class,快速实现监听器类的宏
#define __ILC(Protocol,Message) \
void ___SCN(Protocol,Message)::listen(DataPacket response)

//SII===Software Inner Instruction,软件内部指令
#define SII(Name) \
SII_##Name##_软件内部指令

#pragma endregion

//软件内部命令标识符
enum CmdCode{
    SII(GetRawImage),
    SII(GetTestImage),
    SII(ReadRegister),
    SII(WriteRegister),
    SII(UpImage),
    SII(GenChar),
    SII(GetEnrollImage),
    SII(RegModel),
    SII(StoreChar),
    SII(Search),
    SII(ReadIndexTable),
};

#pragma region ASFComm协议区

__DLC(ASFComm,GetRawImage);
__DLC(ASFComm,GetTestImage);
__DLC(ASFComm,ReadRegister);
__DLC(ASFComm,WriteRegister);
__DLC(ASFComm,ToSleep);
__DLC(ASFComm,ToIdle);
__DLC(ASFComm,Log);
__DLC(ASFComm,AdjustingImage);

enum CmdCodeASFComm{
    //上位机发送的命令
    __SCC(ASFComm,GetRawImage)=0xBB00,
    __SCC(ASFComm,GetTestImage),
    __SCC(ASFComm,ReadRegister),
    __SCC(ASFComm,WriteRegister),
    __SCC(ASFComm,ToSleep),
    __SCC(ASFComm,ToIdle),
    //下位机发送的命令
    __SCC(ASFComm,Log),
    __SCC(ASFComm,AdjustingImage),
};

#pragma endregion

#pragma region Syno协议区

__DLC(Syno,GetImage);
__DLC(Syno,UpImage);
__DLC(Syno,GenChar);
__DLC(Syno,GetEnrollImage);
__DLC(Syno,RegModel);
__DLC(Syno,StoreChar);
__DLC(Syno,Search);
__DLC(Syno,ReadIndexTable);

/*
写命令前,先查文档,给枚举赋值,就代表实现了对应监听器
指令集简化版:
    No	|InstructionName	|EnumVal|功能
    ====|===================|=======|=============================================
    1	|GetImage			|01H	|从传感器上读入图像存于图像缓冲区
    2	|GenChar			|02H	|根据原始图像生成指纹特征存于特征文件缓冲区
    3	|Match				|03H	|精确比对特征文件缓冲区中的特征文件
    4	|Search				|04H	|以特征文件缓冲区中的特征文件搜索整个或部分指纹库
    5	|RegModel			|05H	|将特征文件合并生成模板存于特征文件缓冲区
    6	|StoreChar			|06H	|将特征缓冲区中的文件储存到flash指纹库中
    7	|LoadChar			|07H	|从flash指纹库中读取一个模板到特征缓冲区
    8	|UpChar				|08H	|将特征缓冲区中的文件上传给上位机
    9	|DownChar			|09H	|从上位机下载一个特征文件到特征缓冲区
    10	|UpImage			|0aH	|上传原始图像
    11	|DownImage			|0bH	|下载原始图像
    12	|DeletChar			|0cH	|删除flash指纹库中的一个特征文件
    13	|Empty				|0dH	|清空flash指纹库
    14	|WriteReg			|0eH	|写SOC系统寄存器
    15	|ReadSysPara		|0FH	|读系统基本参数
    16	|SetPwd				|12H	|设置设备握手口令
    17	|VfyPwd				|13H	|验证设备握手口令
    18	|GetRandomCode		|14H	|采样随机数
    19	|SetChipAddr		|15H	|设置芯片地址
    20	|ReadINFpage		|16H	|读取FLASH Information Page内容
    21	|Port_Control		|17H	|通讯端口（UART/USB）开关控制
    22	|WriteNotepad		|18H	|写记事本
    23	|ReadNotepad		|19H	|读记事本
    24	|BurnCode			|1aH	|烧写片内FLASH
    25	|HighSpeedSearch	|1bH	|高速搜索FLASH
    26	|GenBinImage		|1cH	|生成二值化指纹图像
    27	|ValidTempleteNum	|1dH	|读有效模板个数
    28	|ReadIndexTable		|1fH	|读索引表
    29	|GetEnrollImage		|29H	|注册用获取图像
    30	|Cancel				|30H	|取消指令
    31	|AutoEnroll			|31H	|自动注册
    32	|AutoIdentify		|32H	|自动验证
    33	|Sleep				|33H	|休眠指令
    34	|GetChipSN			|34H	|获取芯片唯一序列号
    35	|HandShake			|35H	|握手指令
    36	|CheckSensor		|36H	|校验传感器
    37	|ControlLED			|40H	|控制LED
*/
enum CmdCodeSyno{
    __SCC(Syno,GetImage)=0x01,
    __SCC(Syno,GenChar)=0x02,
    __SCC(Syno,Match),
    __SCC(Syno,Search)=0x04,
    __SCC(Syno,RegModel)=0x05,
    __SCC(Syno,StoreChar)=0x06,
    __SCC(Syno,LoadChar),
    __SCC(Syno,UpChar),
    __SCC(Syno,DownChar),
    __SCC(Syno,UpImage)=0x0a,
    __SCC(Syno,DownImage),
    __SCC(Syno,DeleteChar),
    __SCC(Syno,Empty),
    __SCC(Syno,WriteReg),
    __SCC(Syno,ReadSysPara),
    __SCC(Syno,SetPwd),
    __SCC(Syno,VfyPwd),
    __SCC(Syno,GetRandomCode),
    __SCC(Syno,SetChipAddr),
    __SCC(Syno,ReadINFpage),
    __SCC(Syno,Port_Control),
    __SCC(Syno,WriteNotepad),
    __SCC(Syno,ReadNotepad),
    __SCC(Syno,BurnCode),
    __SCC(Syno,HighSpeedSearch),
    __SCC(Syno,GenBinImage),
    __SCC(Syno,ValidTemplateNum),
    __SCC(Syno,ReadIndexTable)=0x1f,
    __SCC(Syno,GetEnrollImage)=0x29,
    __SCC(Syno,Cancel),
    __SCC(Syno,AutoEnrollImage),
    __SCC(Syno,AutoIdentify),
    __SCC(Syno,GetChipSN),
    __SCC(Syno,HandShake),
    __SCC(Syno,CalibrateSensor),
    __SCC(Syno,生成模组唯一序列号),
    __SCC(Syno,获取模组唯一序列号),
    __SCC(Syno,获取指纹算法库版本号),
    __SCC(Syno,获取固件库版本号),
    __SCC(Syno,ControlLED),
    __SCC(Syno,控制三色LED),
};

#pragma endregion

enum CmdCodeLOG{
    CmdCodeLOG_Info=0xCC00,
    CmdCodeLOG_MeasureTimeStart,
    CmdCodeLOG_MeasureTimeEnd,
    CmdCodeLOG_AdvDbg_AdjImg,
};

enum CmdCodeGD32F30{
    CMD_GET_TEST_IMAGE=0x031F,
    CMD_DEVICE_RESET=0x0320,
    CMD_DETECT_FINGER=0x0321,
    CMD_GET_RAW_IMAGE=0x0322,
    CMD_GET_REDRESS_IMAGE=0x0323,
    CMD_UPLOAD_IMAGE=0x0324,
    CMD_GEN_CHAR=0x0325,
    CMD_MATCH_CHAR=0x0326,
    CMD_STORE_CHAR=0x0327,
    CMD_SEARCH_CHAR=0x0328,
    CMD_DELETE_CHAR=0x0329,
    CMD_EMPTY_CHAR=0x032A,
    CMD_VERIFY_CHAR=0x032B,
    CMD_GET_CHAR=0x032C,
    CMD_PUT_CHAR=0x032D,
    CMD_GET_MBINDEX=0x032E,
    CMD_READ_NOTE_BOOK=0x032F,
    CMD_WRITE_NOTE_BOOK=0x0330,
    CMD_READ_PAR_TABLE=0x0331,
    CMD_SET_BAUD_RATE=0x0332,
    CMD_SET_SECURLEVEL=0x0333,
    CMD_SET_CMOS_PARA=0x0334,
    CMD_RESUME_FACTORY=0x0335,
    CMD_MERGE_CHAR=0x0336,
    CMD_SET_PSW=0x0337,
    CMD_SET_ADDRESS=0x0338,
    CMD_GET_RANDOM=0x0339,
    CMD_DOWNLOAD_IMAGE=0x0340,
    CMD_ERASE_PROGRAM=0x0341,
    CMD_STORE_CHAR_DIRECT=0x0342,
    CMD_READ_CHAR_DIRECT=0x0343,
    CMD_GET_FIRSTVALID_ADD=0x0344,
    CMD_CHIP_ERASE=0x0380,
    CMD_FINGER_DETECT=0X0400,
    CMD_Sleep_MODE=0X0401,
    CMD_SleepToIdle=0x0402,
    CMD_AdjustImage=0x0403,
};

enum CmdCodeACH512{
    USR_CMD_GET_INFO=1,
    USR_CMD_GET_DEVICESN,
    USR_CMD_SET_DEVICESN,
    USR_CMD_GET_SESSION,
    USR_CMD_SET_SESSION,
    USR_CMD_GET_SENSOR_INFO,
    USR_CMD_CONFIG_SENSOR,
    USR_CMD_GRAB,
    USR_CMD_GRAB_NO_CHECK,
    USR_CMD_GRAB_WAIT,
    USR_CMD_GENERATE,
    USR_CMD_MERGE,
    USR_CMD_STORE,
    USR_CMD_SEARCH,
    USR_CMD_MATCH,
    USR_CMD_UP_IMG,
    USR_CMD_DOWN_IMG,
    USR_CMD_LOAD_CHAR,
    USR_CMD_UP_CHAR,
    USR_CMD_DOWN_CHAR,
    USR_CMD_LIST,
    USR_CMD_GET_EMPTY_ID,
    USR_CMD_CHECK_ID,
    USR_CMD_DELETE_ID,
    USR_CMD_REMOVE_ALL,
    USR_CMD_SET_LED,
    USR_CMD_ENTER_IAP,
    USR_CMD_CANCEL,
    USR_CMD_WIRTE_PRODUCT_SESSION,
    USR_CMD_SLEEP,
    USR_CMD_DEV_BOOTLOADER,
    USR_CMD_UP_IMG_EX=0x22
};
