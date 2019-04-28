#pragma once
#include"stdafx.h"
#include"GET_RAW_IMAGE_Listener.h"
#include"GET_TEST_IMAGE_Listener.h"
#include"READ_REGISTER_Listener.h"
#include"WRITE_REGISTER_Listener.h"

CommBoardcast boardcastListener;

CommBoardcast::CommBoardcast(){
    attach(CMD_GET_RAW_IMAGE,(ICommListener*)(new GET_RAW_IMAGE_Listener()));
    attach(CMD_GET_TEST_IMAGE,(ICommListener*)(new GET_TEST_IMAGE_Listener()));
    attach(CMD_READ_NOTE_BOOK,(ICommListener*)(new READ_REGISTER_Listener()));
    attach(CMD_WRITE_NOTE_BOOK,(ICommListener*)(new WRITE_REGISTER_Listener()));
}

void CommBoardcast::attach(int event,ICommListener* listener){
    if(!m.count(event)){
        m[event]=std::vector<ICommListener*>();
    }
    m[event].push_back(listener);
}

void CommBoardcast::boardcast(int event,DataPacket response){
    std::vector<ICommListener*>::iterator it;
    for(it=m[event].begin();it!=m[event].end();it++){
        (*it)->listen(response);
    }
}
