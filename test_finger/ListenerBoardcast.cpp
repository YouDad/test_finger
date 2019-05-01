#pragma once
#include"stdafx.h"

ListenerBoardcast listenerBoardcast;

ListenerBoardcast::ListenerBoardcast(){
    attach(CMD_GET_RAW_IMAGE,new GET_RAW_IMAGE_Listener());
    attach(CMD_GET_TEST_IMAGE,new GET_TEST_IMAGE_Listener());
    attach(CMD_READ_NOTE_BOOK,new READ_REGISTER_Listener());
    attach(CmdCodeLOG_Info,new CmdCodeLOG_Info_Listener());
    attach(CmdCodeLOG_MeasureTimeStart,new CmdCodeLOG_MeasureTimeStart_Listener());
    attach(CmdCodeLOG_MeasureTimeEnd,new CmdCodeLOG_MeasureTimeEnd_Listener());
    attach(CmdCodeLOG_AdvDbg_AdjImg,new CmdCodeLOG_AdvDbg_AdjImg_Listener());
    __BCL(ASFComm,GetRawImage);
    __BCL(ASFComm,GetTestImage);
    __BCL(ASFComm,ReadRegister);
    __BCL(ASFComm,WriteRegister);
    __BCL(ASFComm,ToSleep);
    __BCL(ASFComm,ToIdle);
    __BCL(ASFComm,Log);
    __BCL(ASFComm,AdjustingImage);
}

void ListenerBoardcast::attach(int event,ICommListener* listener){
    if(!m.count(event)){
        m[event]=std::vector<ICommListener*>();
    }
    m[event].push_back(listener);
}

void ListenerBoardcast::boardcast(int event,DataPacket response){
    std::vector<ICommListener*>::iterator it;
    for(it=m[event].begin();it!=m[event].end();it++){
        (*it)->listen(response);
    }
}
