#include "stdafx.h"
// 定义本文件处理的回应是DataPacketSyno这个格式的
typedef DataPacketSyno Response;

// 查看数据包的开头确定协议
bool ResponseConverterSyno::checkProtocol(DataPacket dataPacket){
    if(!dataPacket.haveData()){
        return false;
    }
    unsigned char* head=dataPacket.getPointer();
    return head[0]==0xEF&&head[1]==0x01;
}

// SYNO的数据清洗过程,结果是返回值+数据部分
DataPacket ResponseConverterSyno::convert(DataPacket& data){
    // 因为有大端小端问题,所以每次用数据都先memcpy
    Response response;
    memcpy(&response,data.getPointer(),sizeof(response));

    // 申请缓冲区
    NEWA_INFO;
    uint8_t* arr=new uint8_t[data.readSize()];
    const int header=sizeof(Response)-sizeof(response.data)-sizeof(response.sum);
    int len=0;
    while(checkProtocol(data)){
        // 大小端转化
        response.reconvert();

        // 转移数据,长度为对应的len
        memcpy(arr+len,response.data,response.len);

        len+=response.len;
        data.readData(header+response.len);
        //TODO 校验位
        data.readData(2);
        if(response.sign==SynoSign::EndData){
            break;
        }
        memcpy(&response,data.getPointer(),sizeof(response));
    }
    DataPacket ret(arr,len);
    DELA_INFO;
    delete[] arr;
    return ret;
}

// 返回命令码
int ResponseConverterSyno::getCmdCode(DataPacket data){
    return comm.blockQueue.back().first;
}
