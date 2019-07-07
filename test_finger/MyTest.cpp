#include "stdafx.h"
#include "MyTest.h"

MyTest::MyTest(){
    testLocker.lock();
    testEnd();
}

void MyTest::testBegin(){
    MyLog::SetLimit(MyLog::LOGT);
    testLocker.lock();
    isTest=true;
}

void MyTest::testEnd(){
    My_logs.clear();
    My_CtrlSet.clear();

    isTest=false;
    testLocker.unlock();
}
