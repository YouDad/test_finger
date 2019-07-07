#include "stdafx.h"

namespace UnitTest{

    TEST_CLASS(GD32CommTest){
public:
    TEST_CLASS_INITIALIZE(init){
        conf.InitConfig();
        comm.setBlock(false);
    }

#define CHECK_SEND \
for(int i=0;i<CHECK_Send.size();i++){\
    Assert::AreEqual(CHECK_Send[i],test.commTest.Comm_send[i]);\
}

#define TEST_RECEIVE \
for(int i=0;i<TEST_Receive.size();i++){\
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
for(uint32_t i=0;i<CHECK_Receive.size();i++){\
    Assert::AreEqual((std::string)CHECK_Receive[i],(std::string)test.myTest.My_logs[i]);\
}

    //²âÊÔReadRegister
    TEST_METHOD(ReadRegister){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        TestDataLoader loader(__func__);
        comm.connect(0,0);
        auto TEST_Send=loader.ReadByteSection("TEST Send");
        auto CHECK_Send=loader.ReadByteSection("CHECK Send");
        auto TEST_Receive=loader.ReadByteSection("TEST Receive");
        auto CHECK_Receive=loader.ReadStrSection("CHECK Receive");
        // TEST Send
        comm.request(SII(ReadRegister),DataPacket(TEST_Send));
        while(!test.commTest.Comm_sendSize);
        CHECK_SEND;
        TEST_RECEIVE;
        CHECK_RECEIVE;
        Assert::AreEqual(75,test.uiTest.UI_progress);
        Assert::AreEqual("2",test.myTest.My_CtrlSet[0].second);
        test.testEnd();
    }

    //²âÊÔWriteRegister
    TEST_METHOD(WriteRegister){
        test.testBegin(test.Comm|test.UI,GD32);
        TestDataLoader loader(__func__);
        comm.connect(0,0);
        auto TEST_Send=loader.ReadByteSection("TEST Send");
        auto CHECK_Send=loader.ReadByteSection("CHECK Send");
        // TEST Send
        comm.request(SII(WriteRegister),DataPacket(TEST_Send));
        while(!test.commTest.Comm_sendSize);
        CHECK_SEND;
        test.testEnd();
    }

    TEST_METHOD(GetRawImage){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        TestDataLoader loader(__func__);
        comm.connect(0,0);
        auto CHECK_Send=loader.ReadByteSection("CHECK Send");
        auto TEST_Receive=loader.ReadByteSection("TEST Receive");
        auto CHECK_Receive=loader.ReadStrSection("CHECK Receive");
        // TEST Send
        comm.request(SII(GetRawImage));
        while(!test.commTest.Comm_sendSize);
        CHECK_SEND;
        TEST_RECEIVE;
        CHECK_RECEIVE;
        Assert::AreEqual(100,test.uiTest.UI_progress);
        test.testEnd();
    }

    TEST_METHOD(GetTestImage){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        TestDataLoader loader(__func__);
        comm.connect(0,0);
        auto CHECK_Send=loader.ReadByteSection("CHECK Send");
        auto TEST_Receive=loader.ReadByteSection("TEST Receive");
        auto CHECK_Receive=loader.ReadStrSection("CHECK Receive");
        // TEST Send
        comm.request(SII(GetTestImage));
        while(!test.commTest.Comm_sendSize);
        CHECK_SEND;
        TEST_RECEIVE;
        CHECK_RECEIVE;
        Assert::AreEqual(100,test.uiTest.UI_progress);
        test.testEnd();
    }

    };
}