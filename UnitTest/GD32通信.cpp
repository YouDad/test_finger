#include "stdafx.h"

namespace UnitTest{

    TEST_CLASS(GD32CommTest){
public:

    //测试ReadRegister
    TEST_METHOD(ReadRegister){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        // TEST Send
        uint8_t data[]={0x2F};
        comm.connect(0,0);
        comm.request(SII(ReadRegister),DataPacket(data,sizeof data));
        while(!test.commTest.Comm_sendSize);
        // CHECK Send
        uint8_t sendData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x03,0x02,0x01,0x00,0x2F,0x2D,0xD8};
        for(int i=0;i<sizeof sendData;i++){
            Assert::AreEqual(sendData[i],test.commTest.Comm_send[i]);
        }
        // TEST Receive
        uint8_t receiveData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2F,0x03,0x02,0x01,0x00,0x00,0x00,0x02};
        memcpy(test.commTest.Comm_receive,receiveData,sizeof receiveData);
        bool process=false;
        flow.clear();
        flow.add(0,
            [&](int& result)->bool{
                process=true;
                flow.clear();
                return false;
            }
        );
        flow.jump(0);
        test.commTest.Comm_receiveSize=sizeof receiveData;
        while(!process);
        // CHECK Receive
        Assert::AreEqual(75,test.uiTest.UI_progress);
        Assert::AreEqual("接收到数据包,大小为1",test.myTest.My_logs[0]);
        Assert::AreEqual("线程向主线程发送消息WM_READ_REGISTER",test.myTest.My_logs[1]);
        Assert::AreEqual("接收数据成功",test.myTest.My_logs[2]);
        Assert::AreEqual("2",test.myTest.My_CtrlSet[0].second);
        test.testEnd();
    }

    //测试WriteRegister
    TEST_METHOD(WriteRegister){
        test.testBegin(test.Comm|test.UI,GD32);
        // TEST Send
        uint8_t data[]={0x2F,0x00};
        comm.connect(0,0);
        comm.request(SII(WriteRegister),DataPacket(data,sizeof data));
        while(!test.commTest.Comm_sendSize);
        // CHECK Send
        uint8_t sendData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x03,0x02,0x02,0x00,0x2F,0x00,0x3B,0x41};
        for(int i=0;i<sizeof sendData;i++){
            Assert::AreEqual(sendData[i],test.commTest.Comm_send[i]);
        }
        test.testEnd();
    }

    TEST_METHOD(GetRawImage){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        // TEST Send
        comm.connect(0,0);
        comm.request(SII(GetRawImage));
        while(!test.commTest.Comm_sendSize);
        // CHECK Send
        uint8_t sendData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0x03,0x02,0x00,0x00,0x52,0xB8};
        for(int i=0;i<sizeof sendData;i++){
            Assert::AreEqual(sendData[i],test.commTest.Comm_send[i]);
        }
        // TEST Receive
        uint8_t receiveData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x22,0x03,0x02,0x10,0x02,0x00,0x00};
        int k=0;
        for(int i=0;i<25600;){
            uint16_t* len=(uint16_t*)(receiveData+15);
            if(25600-i<=528){
                *len=25600-i;
            }
            memcpy(test.commTest.Comm_receive+k,receiveData,sizeof receiveData);
            k+=sizeof receiveData;
            for(int j=0;j<528&&i<25600;j++){
                test.commTest.Comm_receive[k++]=i++;
            }
            // TODO-When CRCVal be used
            test.commTest.Comm_receive[k++]=0;
            test.commTest.Comm_receive[k++]=0;
        }
        bool process=false;
        flow.clear();
        flow.add(0,
            [&](int& result)->bool{
                process=true;
                flow.clear();
                return false;
            }
        );
        flow.jump(0);
        test.commTest.Comm_receiveSize=k;
        while(!process);
        // CHECK Receive
        Assert::AreEqual(100,test.uiTest.UI_progress);
        Assert::AreEqual("接收到数据包,大小为25600",test.myTest.My_logs[0]);
        Assert::AreEqual("线程向主线程发送消息WM_GET_RAW_IMAGE",test.myTest.My_logs[1]);
        Assert::AreEqual("接收到160x160的图像",test.myTest.My_logs[2]);
        Assert::AreEqual("加载图片完成",test.myTest.My_logs[3]);
        Assert::AreEqual("接收数据成功",test.myTest.My_logs[4]);
        test.testEnd();
    }

    TEST_METHOD(GetTestImage){
        test.testBegin(test.Comm|test.UI|test.My,GD32);
        // TEST Send
        comm.connect(0,0);
        comm.request(SII(GetTestImage));
        while(!test.commTest.Comm_sendSize);
        // CHECK Send
        uint8_t sendData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x03,0x02,0x00,0x00,0xC6,0x95};
        for(int i=0;i<sizeof sendData;i++){
            Assert::AreEqual(sendData[i],test.commTest.Comm_send[i]);
        }
        // TEST Receive
        uint8_t receiveData[]={0xEF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x03,0x02,0x10,0x02,0x00,0x00};
        int k=0;
        for(int i=0;i<25600;){
            uint16_t* len=(uint16_t*)(receiveData+15);
            if(25600-i<=528){
                *len=25600-i;
            }
            memcpy(test.commTest.Comm_receive+k,receiveData,sizeof receiveData);
            k+=sizeof receiveData;
            for(int j=0;j<528&&i<25600;j++){
                test.commTest.Comm_receive[k++]=i++;
            }
            // TODO-When CRCVal be used
            test.commTest.Comm_receive[k++]=0;
            test.commTest.Comm_receive[k++]=0;
        }
        bool process=false;
        flow.clear();
        flow.add(0,
            [&](int& result)->bool{
                process=true;
                flow.clear();
                return false;
            }
        );
        flow.jump(0);
        test.commTest.Comm_receiveSize=k;
        while(!process);
        // CHECK Receive
        Assert::AreEqual(100,test.uiTest.UI_progress);
        Assert::AreEqual("接收到数据包,大小为25600",test.myTest.My_logs[0]);
        Assert::AreEqual("线程向主线程发送消息WM_GET_TEST_IMAGE",test.myTest.My_logs[1]);
        Assert::AreEqual("接收到160x160的图像",test.myTest.My_logs[2]);
        Assert::AreEqual("加载图片完成",test.myTest.My_logs[3]);
        Assert::AreEqual("接收背景数据成功",test.myTest.My_logs[4]);
        test.testEnd();
    }

    };
}