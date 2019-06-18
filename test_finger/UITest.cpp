#include "stdafx.h"
#include "UITest.h"

UITest::UITest(){
    testLocker.lock();
    testEnd();
}

void UITest::testBegin(){
    testLocker.lock();
    isTest=true;
}

void UITest::testEnd(){
    UI_protocol=0;
    UI_progress=0;

    isTest=false;
    testLocker.unlock();
}
