#include "stdafx.h"
typedef DataPacketSyno Request;


bool RequestConverterSyno::checkProtocol(DataPacket dataPacket){
    return getText(cmbProtocolType)=="Syno";
}

std::queue<int> lastCmdCode;

//Karl<2019年5月19日13:21:47>悟:
//用异常的好处:
//不用if/else而用try/catch
//可以分开正常业务逻辑和例外情况,提高可读性
//而且不用设计异常情况的返回值

int ToSyno(int cmdCode){
    switch(cmdCode){
        case SII(GetRawImage):
            return __SCC(Syno,GetImage);
        case SII(UpImage):
            return __SCC(Syno,UpImage);
        case SII(GenChar):
            return __SCC(Syno,GenChar);
        case SII(RegModel):
            return __SCC(Syno,RegModel);
        case SII(StoreChar):
            return __SCC(Syno,StoreChar);
        case SII(GetEnrollImage):
            return __SCC(Syno,GetEnrollImage);
        case SII(Search):
            return __SCC(Syno,Search);
        case SII(ReadIndexTable):
            return __SCC(Syno,ReadIndexTable);
        default:
            ASF_ERROR(6);
            throw 0;
    }
}

void getSum(Request& r){
    unsigned char* p=(unsigned char*)&r;
    r.sum=0;
    for(int i=6;i<6+3+r.len;i++){
        r.sum+=p[i];
    }
}

std::vector<DataPacket> RequestConverterSyno::convert(int cmdCode,uint8_t* data,uint16_t len){
    std::vector<DataPacket> ret;
    try{
        cmdCode=ToSyno(cmdCode);
        while(1){
            WaitForSingleObject(lastCmdCodeMutex,INFINITE);
            if(lastCmdCode.size()==0){
                lastCmdCode.push(cmdCode);
                ReleaseMutex(lastCmdCodeMutex);
                break;
            } else{
                ReleaseMutex(lastCmdCodeMutex);
                continue;
            }
        }
    } catch(...){
        return ret;
    }

    Request request;

    const int headLength=sizeof(request)-sizeof(request.data);
    const int dataLength=sizeof(request.data);

    memcpy(request.head,"\xEF\x01",2);
    memset(&request.address,-1,sizeof(request.address));

    uint8_t* cmd_data=new uint8_t[len+1];
    cmd_data[0]=cmdCode;
    memcpy(cmd_data+1,data,len);
    len+=1;

    request.len=dataLength;
    request.sign=SynoSign::Command;

    uint8_t* p=cmd_data;
    do{
        if(len<=dataLength){
            request.len=len;
        }
        memcpy(request.data,p,request.len);
        p+=request.len;
        len-=request.len;
        getSum(request);
        int before_len=request.len;
        request.convert();
        ret.push_back(DataPacket(&request,headLength+before_len));
    } while(len>0);
    delete[] cmd_data;
    return ret;
}
