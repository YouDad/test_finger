#pragma once
#include"stdafx.h"

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

#pragma pack(1)
struct DataPacketGD32F30{
    uint16_t	Head;
    uint32_t	Addr;
    uint32_t	Password;
    uint16_t	NO;
    uint16_t	CMD;
    uint8_t		Sign;
    uint16_t	Length;
    uint8_t		Sendbuf[530];
};
struct ResponsePacketGD32F30{
    uint16_t	Head;
    uint32_t	Addr;
    uint32_t	Password;
    uint16_t	NO;
    uint16_t	CMD;
    uint8_t		Sign;
    uint16_t	Length;
    uint16_t    ACK;
    uint8_t		Sendbuf[530];
};
#pragma pack(4)

class RequestConverterGD32F30:public ICommProtocolRequestConverter{
public:
    virtual bool checkProtocol(uint16_t head) override;
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t * Data,uint16_t Len) override;
};
