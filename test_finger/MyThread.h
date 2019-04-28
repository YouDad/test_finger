#pragma once
#include"stdafx.h"

#define Function_t(ReturnType,Param1) std::function<ReturnType(Param1)>
#define DefFunction(ReturnType,Param1) [&](Param1)->ReturnType

#define ThreadFunction_t Function_t(void,void)
#define VoidFunction(Param1) DefFunction(void,Param1)
#define ThreadFunction__(name) VoidFunction

class MyThread{
public:
    MyThread(ThreadFunction_t pFunction);
    bool start();
    bool terminate();
    bool isRun();
private:
    HANDLE thread=0;
    ThreadFunction_t pf;
    static DWORD WINAPI run(LPVOID params);
};
