#pragma once
#include"stdafx.h"

#define _Function_t(ReturnType,...) std::function<ReturnType(__VA_ARGS__)>
#define _Function(ReturnType,...) [&](__VA_ARGS__)->ReturnType

#define ThreadFunction_t _Function_t(void,void)
#define VoidFunction(Param1) _Function(void,Param1)
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
