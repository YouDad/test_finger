#pragma once
#include"stdafx.h"

class DataPacket{
public:
    DataPacket(const void* d,int l);
    DataPacket();

    //用完之后调用
    void Destruction();
    bool isValid();
    int size();
    BYTE* getPointer();
    BYTE* getNewArray();
    void readData(int readByteSize);
    bool haveData();
private:
    //数据包
    BYTE* data;
    //数据包长度
    int   len;
    //已读字节数
    int   read;
};