#pragma once
#include <afxwin.h>
#include "MyDefine.h"
#include "MyString.h"
#include "MyLocker.h"

class CommTest{
public:
    bool isTest;
    MyLocker testLocker;

    uint8_t Comm_send[65536];
    int Comm_sendSize;
    uint8_t Comm_receive[65536];
    int Comm_receiveSize;

    CommTest();
    void testBegin();
    void testEnd();
};