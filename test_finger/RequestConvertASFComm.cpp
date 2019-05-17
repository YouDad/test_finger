#include "stdafx.h"
typedef struct DataPacketASFComm Request;


bool RequestConverterASFComm::checkProtocol(DataPacket dataPacket){
    return getText(cmbProtocolType)=="ASFComm";
}

int ToASFComm(int cmdCode){
    switch(cmdCode){
        case CmdCode_GetTestImage:
            return __SCC(ASFComm,GetTestImage);
        case CmdCode_GetRawImage:
            return __SCC(ASFComm,GetRawImage);
        case CmdCode_ReadRegister:
            return __SCC(ASFComm,ReadRegister);
        case CmdCode_WriteRegister:
            return __SCC(ASFComm,WriteRegister);
        default:
            ASF_ERROR(4);
            return cmdCode;
    }
}

std::vector<DataPacket> RequestConverterASFComm::convert(int cmdCode,uint8_t* data,uint16_t len){
    cmdCode=ToASFComm(cmdCode);
    std::vector<DataPacket> ret;

    Request request;

    const int headLength=sizeof(request)-sizeof(request.data);
    const int dataLength=sizeof(request.data);

    memcpy(request.head,"ASF",3);
    request.cmd=cmdCode;
    request.len=dataLength;
    request.sign=DataNotEnd;

    uint8_t* p=data;
    do{
        if(len<=dataLength){
            request.sign=DataEnd;
            request.len=len;
        }
        memcpy(request.data,p,request.len);
        p+=request.len;
        len-=request.len;
        ret.push_back(DataPacket(&request,headLength+request.len));
    } while(len>0);
    return ret;
}
