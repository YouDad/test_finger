#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest{

    TEST_CLASS(DataPacketUnitTest){
public:
    struct Mock{
        //数据包
        uint8_t* data;
        //数据包长度
        int   len;
        //已读字节数
        int   read;
    };
    Mock getMock(DataPacket packet){
        Mock ret;
        Assert::AreEqual(sizeof packet,12u);
        memcpy(&ret,&packet,sizeof packet);
        return ret;
    }
    DataPacket getPacket(Mock mock){
        DataPacket packet;
        Assert::AreEqual(sizeof packet,12u);
        memcpy(&packet,&mock,sizeof packet);
        return packet;
    }

    //测试无参构造函数
    TEST_METHOD(DataPacket1){
        DataPacket packet;
        Mock mock=getMock(packet);
        Assert::AreEqual(-1,mock.len);
        Assert::AreEqual(0,(int)mock.data);
        Assert::AreEqual(0,mock.read);
        packet.Destruction();
    }

    //测试有参构造函数
    TEST_METHOD(DataPacket2){
        char* str=new char[4];
        strcpy(str,"str");
        DataPacket packet(str,4);
        Mock mock=getMock(packet);
        Assert::AreEqual(4,mock.len);
        Assert::AreNotEqual((char*)mock.data,str);
        Assert::AreEqual(0,mock.read);
        Assert::AreEqual(0,strcmp((char*)mock.data,str));
        packet.Destruction();
        delete[] str;
    }

    };
}