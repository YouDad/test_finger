#include "stdafx.h"

MyLocker::MyLocker(int now,int sum){
    mutex=CreateSemaphore(0,now,sum,0);
    this->now=now;
}

void MyLocker::lock(){
    WaitForSingleObject(mutex,-1);
    now--;
}

void MyLocker::unlock(){
    ReleaseSemaphore(mutex,1,0);
    now++;
}

bool MyLocker::islock(){
    return now==0;
}
