#include "stdafx.h"

namespace UnitTest{

    TEST_CLASS(TestDataLoaderTest){
public:
    //测试DataLoader
    TEST_METHOD(TestDataLoaderTest1){
        TestDataLoader loader(__func__);

        auto a=loader.ReadByteSection("TEST Send");
        Assert::AreEqual(1u,a.size());
        Assert::AreEqual((uint8_t)0x2F,a[0]);

        auto b=loader.ReadByteSection("CHECK Send");
        Assert::AreEqual(2u,b.size());
        Assert::AreEqual((uint8_t)0xEF,b[0]);
        Assert::AreEqual((uint8_t)0x02,b[1]);

        auto c=loader.ReadByteSection("TEST Receive");
        Assert::AreEqual(3u,c.size());
        Assert::AreEqual((uint8_t)0xEF,c[0]);
        Assert::AreEqual((uint8_t)0x02,c[1]);
        Assert::AreEqual((uint8_t)0x00,c[2]);

        auto d=loader.ReadStrSection("CHECK Receive");
        Assert::AreEqual(2u,d.size());
        Assert::AreEqual(true,d[0]=="接据包,大小为1");
        Assert::AreEqual(true,d[1]=="线消息WM_READ_REGISTER");
    }

    };
}