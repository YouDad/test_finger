#include "stdafx.h"

bool RequestConverterACH512::checkProtocol(uint16_t head){
    return false;
}

std::vector<DataPacket> RequestConverterACH512::convert(int CmdCode,uint8_t * Data,uint16_t Len){
    return std::vector<DataPacket>();
}
