#pragma once
#include"stdafx.h"

class RequestConverterACH512:public ICommProtocolRequestConverter<RequestPacketGD32F30>{
public:
    bool checkProtocol(uint16_t head);
};