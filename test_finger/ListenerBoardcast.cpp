#pragma once
#include"stdafx.h"

ListenerBoardcast listenerBoardcast;

ListenerBoardcast::ListenerBoardcast(){
    __BCL(GD32,GD32F30,GetRawImage);
    __BCL(GD32,GD32F30,GetTestImage);
    __BCL(GD32,GD32F30,ReadRegister);
    __BCL(GD32,GD32F30,DeviceInfo);

    __BCL(0,LOG,Info);
    __BCL(0,LOG,MeasureTimeStart);
    __BCL(0,LOG,MeasureTimeEnd);
    __BCL(0,LOG,AdvDbg_AdjImg);

    __BCL(SYNO,Syno,GetImage);
    __BCL(SYNO,Syno,UpImage);
    __BCL(SYNO,Syno,GetEnrollImage);
    __BCL(SYNO,Syno,GenChar);
    __BCL(SYNO,Syno,RegModel);
    __BCL(SYNO,Syno,StoreChar);
    __BCL(SYNO,Syno,Search);
    __BCL(SYNO,Syno,ReadIndexTable);
    __BCL(SYNO,Syno,Match);
    __BCL(SYNO,Syno,LoadChar);
    __BCL(SYNO,Syno,DeleteChar);
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
        if((*it)->accept()){
            (*it)->listen(response);
        }
    }
}

ICommListener::ICommListener(const char* protocol){
    this->protocol=protocol;
}

bool ICommListener::accept(){
    if(protocol==0){
        return true;
    } else{
        return getText(cmbProtocolType)==protocol;
    }
}
