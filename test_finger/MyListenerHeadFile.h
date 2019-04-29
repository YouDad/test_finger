#pragma once
#include"GET_RAW_IMAGE.h"
#include"GET_TEST_IMAGE.h"
#include"READ_REGISTER.h"
#include"CmdCodeLOG_INFO.h"
#include"CmdCodeLOG_MeasureTime.h"

enum CmdCodeLOG{
    CmdCodeLOG_Info=0xCC00,
    CmdCodeLOG_MeasureTimeStart,
    CmdCodeLOG_MeasureTimeEnd,
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
