#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest{

    TEST_CLASS(DataPacketUnitTest){
public:

    //测试无参构造函数
    TEST_METHOD(DataPacket1){
        DataPacket packet;
        Assert::AreEqual(-1,packet.len);
        Assert::AreEqual(0,(int)packet.data);
        Assert::AreEqual(0,packet.read);
        packet.Destruction();
    }

    //测试有参构造函数
    TEST_METHOD(DataPacket2){
        char* str=new char[4];
        strcpy(str,"str");
        DataPacket packet(str,4);
        Assert::AreEqual(4,packet.len);
        Assert::AreNotEqual((char*)packet.data,str);
        Assert::AreEqual(0,packet.read);
        Assert::AreEqual(0,strcmp((char*)packet.data,str));
        packet.Destruction();
        delete[] str;
    }

    };
}