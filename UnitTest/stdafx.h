// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "targetver.h"

// 用于 CppUnitTest 的头文件
#include "CppUnitTest.h"

// TODO: 在此处引用程序需要的其他头文件
#define private public
#include "../test_finger/stdafx.h"
#undef private

#include "TestDataLoader.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define CHECK_SEND \
for(uint32_t i=0;i<CHECK_Send.size();i++){\
    Assert::AreEqual(CHECK_Send[i],test.commTest.Comm_send[i]);\
}

#define TEST_RECEIVE \
for(uint32_t i=0;i<TEST_Receive.size();i++){\
    test.commTest.Comm_receive[i]=TEST_Receive[i];\
}\
bool process=false;\
flow.clear();\
flow.add(0,\
    [&](int& result)->bool{\
        process=true;\
        flow.clear();\
        return false;\
    }\
);\
flow.jump(0);\
test.commTest.Comm_receiveSize=TEST_Receive.size();\
while(!process);

#define CHECK_RECEIVE \
Assert::AreEqual(CHECK_Receive.size(),test.myTest.My_logs.size());\
for(uint32_t i=0;i<CHECK_Receive.size();i++){\
    Assert::AreEqual((std::string)CHECK_Receive[i],(std::string)test.myTest.My_logs[i]);\
}
