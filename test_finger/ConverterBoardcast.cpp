#include"stdafx.h"

// 全局唯一转化器广播
ConverterBoardcast converterBoardcast;

// 构造器
ConverterBoardcast::ConverterBoardcast(){
    attach(new RequestConverterSyno());
    attach(new ResponseConverterSyno());

    attach(new RequestConverterGD32F30());
    attach(new ResponseConverterGD32F30());

    attach(new ResponseConverterLOG());
}

// 增加监听的请求转化器
void ConverterBoardcast::attach(ICommProtocolRequestConverter * converter){
    requestVector.push_back(converter);
}

// 增加监听的响应转化器
void ConverterBoardcast::attach(ICommProtocolResponseConverter * converter){
    responseVector.push_back(converter);
}

// 获得请求转化器
ICommProtocolRequestConverter* ConverterBoardcast::RequestConvert(){
    auto&v=requestVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(DataPacket())){
            return *it;
        }
    }
    return NULL;
}

// 清洗数据包,返回其中的数据
DataPacket ConverterBoardcast::ResponseConvert(DataPacket & dataPacket){
    auto&v=responseVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(dataPacket)){
            return (*it)->convert(dataPacket);
        }
    }
    return DataPacket();
}

// 根据数据包,返回其中命令码
int ConverterBoardcast::ResponseGetCmdCode(DataPacket dataPacket){
    auto&v=responseVector;
    for(auto it=v.begin();it!=v.end();it++){
        if((*it)->checkProtocol(dataPacket)){
            return (*it)->getCmdCode(dataPacket);
        }
    }
    return -1;
}
