#pragma once

class DataPacket{
public:
    DataPacket(const void* d,int l);
    DataPacket();

    //用完之后调用
    void Destruction();
    //是否有效
    bool isValid();
    //包内数据大小
    int size();
    //包内还剩多少数据
    int readSize();
    //获得可读数据的起始地址
    uint8_t* getPointer();
    //读掉一些数据
    void readData(int readByteSize);
    //判断是否还有可读数据
    bool haveData();
private:
    //数据包
    uint8_t* data;
    //数据包长度
    int   len;
    //已读字节数
    int   read;
};