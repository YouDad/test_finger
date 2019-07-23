#include "stdafx.h"
// 定义本文件处理的回应是DataPacketLOG这个格式的
typedef DataPacketLOG Response;

// 查看数据包的开头确定协议
bool ResponseConverterLOG::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    uint8_t* p=dataPacket.getPointer();
    return p[0]=='L'&&p[1]=='O'&&p[2]=='G';
}

// LOG的数据清洗过程,结果是数据部分
DataPacket ResponseConverterLOG::convert(DataPacket& data){
    const int size=Response::Header+Response::Checker;
    int totalLength=0;
    NEWA_INFO;
    uint8_t* tmpArray=new uint8_t[data.readSize()];
    auto pData=(Response*)data.getPointer();
    while(checkProtocol(data)){
        memcpy(tmpArray+totalLength,pData->Data,pData->Len);
        totalLength+=pData->Len;
        data.readData(size+pData->Len);
        if(pData->Sign==DataEnd){
            break;
        }
        pData=(Response*)data.getPointer();
    }
    DataPacket ret(tmpArray,totalLength);
    DELA_INFO;
    delete[] tmpArray;
    return ret;
}

// 返回命令码
int ResponseConverterLOG::getCmdCode(DataPacket data){
    return ((Response*)data.getPointer())->Cmd;
}
