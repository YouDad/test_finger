#pragma once
#include"stdafx.h"

enum CmdCodeGD32F30{
    //GET_TEST_IMAGE=0x031F,
    //DEVICE_RESET=0x0320,
    //DETECT_FINGER=0x0321,
    //GET_RAW_IMAGE=0x0322,
    //GET_REDRESS_IMAGE=0x0323,
    //UPLOAD_IMAGE=0x0324,
    //GEN_CHAR=0x0325,
    //MATCH_CHAR=0x0326,
    //STORE_CHAR=0x0327,
    //SEARCH_CHAR=0x0328,
    //DELETE_CHAR=0x0329,
    //EMPTY_CHAR=0x032A,
    //VERIFY_CHAR=0x032B,
    //GET_CHAR=0x032C,
    //PUT_CHAR=0x032D,
    //GET_MBINDEX=0x032E,
    //READ_NOTE_BOOK=0x032F,
    //WRITE_NOTE_BOOK=0x0330,
    //READ_PAR_TABLE=0x0331,
    //SET_BAUD_RATE=0x0332,
    //SET_SECURLEVEL=0x0333,
    //SET_CMOS_PARA=0x0334,
    //RESUME_FACTORY=0x0335,
    //MERGE_CHAR=0x0336,
    //SET_PSW=0x0337,
    //SET_ADDRESS=0x0338,
    //GET_RANDOM=0x0339,
    //DOWNLOAD_IMAGE=0x0340,
    //ERASE_PROGRAM=0x0341,
    //STORE_CHAR_DIRECT=0x0342,
    //READ_CHAR_DIRECT=0x0343,
    //GET_FIRSTVALID_ADD=0x0344,
    //CHIP_ERASE=0x0380,
    //FINGER_DETECT=0X0400,
    //Sleep_MODE=0X0401,
    //SleepToIdle=0x0402,
    //AdjustImage=0x0403,
};

#pragma pack(1)
struct RequestPacketGD32F30{
    uint16_t	Head;
    uint32_t	Addr;
    uint32_t	Password;
    uint16_t	NO;
    uint16_t	CMD;
    uint8_t		Sign;
    uint16_t	Length;
    uint8_t		Sendbuf[530];
};
#pragma pack(4)

class RequestConverterGD32F30:public ICommProtocolRequestConverter<CmdCodeGD32F30>{
public:
    bool checkProtocol(uint16_t head);
    std::vector<DataPacket> convert(CmdCodeGD32F30 CmdCode,uint8_t * Data,uint16_t Len);
};
