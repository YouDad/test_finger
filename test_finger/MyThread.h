#pragma once
#include <functional>
#include <Windows.h>

// 线程回调函数类型
#define ThreadFunction_t std::function<void(void)>
#define ThreadFunction [&]()

class MyThread{
public:
    // 如果tmp为真,那么线程结束后会自动释放,所以只有new没有delete
    MyThread(ThreadFunction_t pFunction,bool tmp=false);
    // 启动线程
    bool start();
    // 关闭线程
    bool terminate();
    // 返回线程是否运行
    bool isRun();
private:
    // 线程句柄
    HANDLE thread=0;
    // 线程回调函数
    ThreadFunction_t pf;
    // 是否是自释放线程
    bool temporary;
    // 普通线程运行函数
    static DWORD WINAPI run(LPVOID params);
    // 自释放线程运行函数
    static DWORD WINAPI temporaryRun(LPVOID params);
};
