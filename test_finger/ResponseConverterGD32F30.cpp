#include "stdafx.h"
// 定义本文件处理的回应是ResponsePacketGD32F30这个格式的
typedef ResponsePacketGD32F30 Response;

// 查看数据包的开头确定协议
bool ResponseConverterGD32F30::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    uint8_t*p=dataPacket.getPointer();
    return p[0]==0xEF&&p[1]==0x02;
}

// GD32的数据清洗过程,结果是数据部分
DataPacket ResponseConverterGD32F30::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int tmpLength=0;

    // 申请缓冲区
    uint8_t* tmpArray=new uint8_t[data.readSize()];
    Response* pData=(Response*)data.getPointer();
    while(checkProtocol(data)){
        // 转移数据,长度为对应的Length
        memcpy(tmpArray+tmpLength,pData->Sendbuf,pData->Length);
        tmpLength+=pData->Length;
        data.readData(size+pData->Length);
        pData=(Response*)data.getPointer();
    }
    DataPacket ret(tmpArray,tmpLength);
    delete[] tmpArray;
    return ret;
}

// 返回命令码
int ResponseConverterGD32F30::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->CMD;
}
