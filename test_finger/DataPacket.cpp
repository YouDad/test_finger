#include"stdafx.h"

DataPacket::DataPacket(const void * d,int l){
    NEWA_INFO;
    data=new uint8_t[l+1];
    memset(data,0,l+1);
    len=l;
    read=0;
    memcpy(data,d,l);
}

DataPacket::DataPacket(std::vector<uint8_t>& v){
    len=v.size();
    NEWA_INFO;
    data=new uint8_t[len];
    memset(data,0,len);
    read=0;
    for(int i=0;i<len;i++){
        data[i]=v[i];
    }
}

DataPacket::DataPacket(){
    data=0;
    len=0;
    read=0;
}

// 用完之后调用
void DataPacket::Destruction(){
    if(isValid()){
        DELA_INFO;
        delete[] data;
        len=0;
        read=0;
    }
}

// 是否有效
bool DataPacket::isValid(){
    return len>read;
}

// 包内数据大小
int DataPacket::size() const{
    return len;
}

// 包内还剩多少数据
int DataPacket::readSize() const{
    return len-read;
}

// 获得可读数据的起始地址
uint8_t * DataPacket::getPointer() const{
    return data+read;
}

// 读掉一些数据
void DataPacket::readData(int readByteSize){
    read+=readByteSize;
}

// 判断是否还有可读数据
bool DataPacket::haveData() const{
    return len>read;
}
