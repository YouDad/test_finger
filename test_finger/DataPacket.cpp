#include"stdafx.h"
#include"DataPacket.h"

DataPacket::DataPacket(const void * d,int l){
    data=new uint8_t[l+1];
    memset(data,0,l+1);
    len=l;
    read=0;
    memcpy(data,d,l);
}

DataPacket::DataPacket(){
    data=0;
    len=-1;
    read=0;
}

void DataPacket::Destruction(){
    if(isValid()){
        delete data;
        len=-1;
        read=0;
    }
}

bool DataPacket::isValid(){
    return len!=-1&&len>read;
}

int DataPacket::size(){
    return len;
}

int DataPacket::readSize(){
    return len-read;
}

uint8_t * DataPacket::getPointer(){
    return data+read;
}

void DataPacket::readData(int readByteSize){
    read+=readByteSize;
}

bool DataPacket::haveData(){
    return read!=len;
}
