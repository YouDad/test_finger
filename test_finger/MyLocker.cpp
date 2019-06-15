#include "stdafx.h"

// 共享资源计数
MyLocker::MyLocker(int now,int sum){
    mutex=CreateSemaphore(0,now,sum,0);
    this->now=now;
}

// P原语
void MyLocker::lock(){
    WaitForSingleObject(mutex,-1);
    now--;
}

// V原语
void MyLocker::unlock(){
    ReleaseSemaphore(mutex,1,0);
    now++;
}

// 返回true代表该锁的资源被耗尽
bool MyLocker::islock(){
    return now==0;
}
