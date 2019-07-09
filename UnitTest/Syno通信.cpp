#include "stdafx.h"

namespace IntegrationTest{

    TEST_CLASS(SynoCommTest){
public:
    TEST_CLASS_INITIALIZE(init){
        conf.InitConfig();
        comm.setBlock(true);
    }

    //≤‚ ‘GetRawImage
    TEST_METHOD(GetRawImage1){
        test.testBegin(test.Comm|test.UI|test.My,SYNO);
        TestDataLoader loader(__FUNCTION__);
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
        test.testEnd();
    }

    //≤‚ ‘GetRawImage
    TEST_METHOD(GetRawImage2){
        test.testBegin(test.Comm|test.UI|test.My,SYNO);
        TestDataLoader loader(__FUNCTION__);
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
        test.testEnd();
    }

    //≤‚ ‘UpImage
    TEST_METHOD(UpImage){
        test.testBegin(test.Comm|test.UI|test.My,SYNO);
        TestDataLoader loader(__FUNCTION__);
        comm.connect(0,0);
        auto CHECK_Send=loader.ReadByteSection("CHECK Send");
        auto TEST_Receive=loader.ReadByteSection("TEST Receive");
        auto CHECK_Receive=loader.ReadStrSection("CHECK Receive");
        // TEST Send
        comm.request(SII(UpImage));
        while(!test.commTest.Comm_sendSize);
        CHECK_SEND;
        TEST_RECEIVE;
        test.myTest.My_logs;
        CHECK_RECEIVE;
        test.testEnd();
    }

    };
}