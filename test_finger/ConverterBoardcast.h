#pragma once
#include"stdafx.h"

// 协议转化器接口
class ICommProtocolConverter{
public:
    // 确认是否是需要处理的协议
    virtual bool checkProtocol(DataPacket dataPacket)=0;
};

// 请求转化器接口
class ICommProtocolRequestConverter:public ICommProtocolConverter{
public:
    // 把命令+数据转化为字节流
    virtual std::vector<DataPacket> convert(int CmdCode,uint8_t* Data,uint16_t Len)=0;
};

// 响应转化器接口
class ICommProtocolResponseConverter:public ICommProtocolConverter{
public:
    // 清洗数据包,提取其中的数据
    virtual DataPacket convert(DataPacket& data)=0;
    // 根据数据包,返回其中的命令码
    virtual int getCmdCode(DataPacket data)=0;
};

// 转化器广播
class ConverterBoardcast{
public:
    // 构造器
    ConverterBoardcast();
    // 获得请求转化器
    ICommProtocolRequestConverter* RequestConvert();
    // 清洗数据包,返回其中的数据
    DataPacket ResponseConvert(DataPacket& data);
    // 根据数据包,返回其中命令码
    int ResponseGetCmdCode(DataPacket data);
private:
    // 增加监听的请求转化器
    void attach(ICommProtocolRequestConverter* converter);
    // 增加监听的响应转化器
    void attach(ICommProtocolResponseConverter* converter);
    // 请求转化器向量
    std::vector<ICommProtocolRequestConverter*> requestVector;
    // 响应转化器向量
    std::vector<ICommProtocolResponseConverter*> responseVector;
};

// 全局唯一转化器广播
extern ConverterBoardcast converterBoardcast;

// GD32的Sign
enum ProtocolSign{
    Unspecified,
    DataNotEnd,
    DataEnd,
};

// Syno的Sign
enum SynoSign{
    Command=1,
    NotEndData=2,
    ACK=7,
    EndData=8,
};
