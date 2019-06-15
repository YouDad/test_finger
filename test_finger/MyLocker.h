#pragma once
#include <Windows.h>

class MyLocker{
public:
    // 共享资源计数
    MyLocker(int now=1,int sum=1);
    // P原语
    void lock();
    // V原语
    void unlock();
    // 返回true代表该锁的资源被耗尽
    bool islock();
private:
    HANDLE mutex;
    int now;
};
