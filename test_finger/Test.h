#pragma once
#include "MyLocker.h"
#include "MyTest.h"
#include "UITest.h"
#include "CommTest.h"

class Test{
public:
    MyLocker testLocker;

    const int Comm=1;
    const int UI=2;
    const int My=4;

    CommTest commTest;
    UITest uiTest;
    MyTest myTest;

    int type;

    Test();
    void testBegin(int type,const char* protocol=0);
    void testEnd();
    bool isTest(int type);
};
extern Test test;
