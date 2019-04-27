#include "CommProtocolConverter.h"

DataPacket::DataPacket(const void * d,int l){
    data=new BYTE[l+1];
    len=l;
    memcpy(data,d,l);
}

void DataPacket::Destruction(){
    delete data;
}
