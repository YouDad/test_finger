#include "stdafx.h"

MyThread::MyThread(ThreadFunction_t pFunction){
    pf=pFunction;
}

bool MyThread::start(){
    thread=CreateThread(0,0,run,&pf,0,0);
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