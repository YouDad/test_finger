#pragma once
#include"stdafx.h"

void CommBoardcast::attach(int event,ICommListener* listener){
    if(!m.count(event)){
        m[event]=std::vector<ICommListener*>();
    }
    m[event].push_back(listener);
}

void CommBoardcast::boardcast(int event,DefaultResponsePacket response){
    std::vector<ICommListener*>::iterator it;
    for(it=m[event].begin();it!=m[event].end();it++){
        (*it)->listen(response);
    }
}

void GET_RAW_IMAGE_Listener::listen(DefaultResponsePacket response){

}
