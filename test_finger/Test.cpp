#include "stdafx.h"
#include "Test.h"

Test test;

Test::Test(){
    commTest.testBegin();
    commTest.testEnd();
    uiTest.testBegin();
    uiTest.testEnd();
    myTest.testBegin();
    myTest.testEnd();
}

void Test::testBegin(int type,const char * protocol){
    testLocker.lock();
    this->type=type|UI;
    if(this->type&Comm){
        commTest.testBegin();
    }
    if(this->type&UI){
        uiTest.UI_protocol=protocol;
        uiTest.testBegin();
    }
    if(this->type&My){
        myTest.testBegin();
    }
}

void Test::testEnd(){
    if(this->type&My){
        myTest.testEnd();
    }
    if(this->type&UI){
        uiTest.testEnd();
    }
    if(this->type&Comm){
        commTest.testEnd();
    }
    type=0;
    testLocker.unlock();
}

bool Test::isTest(int type){
    return this->type&type;
}
