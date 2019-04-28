#pragma once
#include"stdafx.h"

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

#pragma pack(1)
struct HangXinSendPacket{
    char fixed[3];//'z','t','\0'
    uint8_t cmd;
    char fixed2[4];//'\0','\0','\0','\0'
    uint32_t len;
    uint8_t data[242*266+2];
    uint16_t sumVal;
};
#pragma pack(4)

class RequestConverterACH512:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(uint16_t head) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};
