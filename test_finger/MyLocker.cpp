#include "stdafx.h"

locker::locker(int now,int sum){
    mutex=CreateSemaphore(0,now,sum,0);
    this->now=now;
}

void locker::lock(){
    WaitForSingleObject(mutex,-1);
    now--;
}

void locker::unlock(){
    ReleaseSemaphore(mutex,1,0);
    now++;
}

bool locker::islock(){
    return now==0;
}
