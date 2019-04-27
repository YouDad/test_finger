#include"stdafx.h"

DataPacket::DataPacket(const void * d,int l){
    data=new BYTE[l+1];
    len=l;
    memcpy(data,d,l);
}

DataPacket::DataPacket(){
    data=0;
    len=-1;
}

void DataPacket::Destruction(){
    delete data;
    len=-1;
}

bool DataPacket::isValid(){
    return len==-1;
}
