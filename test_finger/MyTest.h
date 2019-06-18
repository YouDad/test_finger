#pragma once
#include <afxwin.h>
#include <vector>
#include "MyString.h"
#include "MyLocker.h"

class MyTest{
public:
    bool isTest;
    MyLocker testLocker;

    std::vector<MyString> My_logs;
    std::vector<std::pair<CWnd*,MyString>> My_CtrlSet;

    MyTest();
    void testBegin();
    void testEnd();
};
