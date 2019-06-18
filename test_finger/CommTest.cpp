#include "stdafx.h"
#include "CommTest.h"

CommTest::CommTest(){
    testLocker.lock();
    testEnd();
}

void CommTest::testBegin(){
    testLocker.lock();
    isTest=true;
}

void CommTest::testEnd(){
    memset(Comm_send,Comm_sendSize=0,sizeof Comm_send);
    memset(Comm_receive,Comm_receiveSize=0,sizeof Comm_receive);

    isTest=false;
    testLocker.unlock();
}
