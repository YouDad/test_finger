#pragma once
#include <functional>
#include <Windows.h>

#define ThreadFunction_t std::function<void(void)>
#define ThreadFunction [&]()

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
