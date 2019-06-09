#include"stdafx.h"

ConverterBoardcast converterBoardcast;

ConverterBoardcast::ConverterBoardcast(){
    attach(new RequestConverterSuUSB());
    attach(new ResponseConverterSuUSB());

    attach(new RequestConverterSyno());
    attach(new ResponseConverterSyno());

    attach(new RequestConverterASFComm());
    attach(new ResponseConverterASFComm());

    attach(new RequestConverterACH512());
    attach(new RequestConverterGD32F30());

    attach(new ResponseConverterGD32F30());
    attach(new ResponseConverterLOG());
}

void ConverterBoardcast::attach(ICommProtocolRequestConverter * converter){
    requestVector.push_back(converter);
}

void ConverterBoardcast::attach(ICommProtocolResponseConverter * converter){
    responseVector.push_back(converter);
}

ICommProtocolRequestConverter* ConverterBoardcast::RequestConvert(){
    auto&v=requestVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(DataPacket())){
            return *it;
        }
    }
    return NULL;
}

DataPacket ConverterBoardcast::ResponseConvert(DataPacket & dataPacket){
    auto&v=responseVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(dataPacket)){
            return (*it)->convert(dataPacket);
        }
    }
    return DataPacket();
}

int ConverterBoardcast::ResponseGetCmdCode(DataPacket dataPacket){
    auto&v=responseVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(dataPacket)){
            return (*it)->getCmdCode(dataPacket);
        }
    }
    return -1;
}
