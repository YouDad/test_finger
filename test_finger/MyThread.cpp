#include "stdafx.h"

MyThread::MyThread(ThreadFunction_t pFunction,bool tmp){
    pf=pFunction;
    temporary=tmp;
}

struct temporaryParam{
    ThreadFunction_t pf;
    MyThread* beDelete;
};

std::vector<temporaryParam> TPVec;

MyLocker getSize(0,1);

bool MyThread::start(){
    if(temporary){
        TPVec.push_back(temporaryParam{pf,this});
        int size=TPVec.size();
        thread=CreateThread(0,0,temporaryRun,&size,0,0);
        getSize.lock();
    } else{
        thread=CreateThread(0,0,run,&pf,0,0);

    }
    return thread!=0;
}

bool MyThread::terminate(){
    if(thread){
        TerminateThread(thread,-1);
        CloseHandle(thread);
        thread=0;
        return true;
    }
    return false;
}

bool MyThread::isRun(){
    return thread!=0;
}

DWORD WINAPI MyThread::run(LPVOID params){
    (*(ThreadFunction_t*)params)();
    return 0;
}

DWORD WINAPI MyThread::temporaryRun(LPVOID params){
    int size=*(int*)params;
    getSize.unlock();
    TPVec[size-1].pf();
    delete TPVec[size-1].beDelete;
    return 0;
}
