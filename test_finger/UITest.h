#pragma once
#include "MyLocker.h"

class UITest{
public:
    bool isTest;
    MyLocker testLocker;

    const char* UI_protocol;
    int UI_progress;

    UITest();
    void testBegin();
    void testEnd();
};
