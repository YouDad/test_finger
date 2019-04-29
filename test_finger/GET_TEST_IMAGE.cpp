#include "stdafx.h"
#include "GET_TEST_IMAGE.h"

void GET_TEST_IMAGE_Listener::listen(DataPacket response){
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"接收到数据包,大小为%d",response.size());
    MyLog.print(Log::LOGD,"线程向主线程发送消息CMD_GET_TEST_IMAGE");
    sendMessage(WM_GET_TEST_IMAGE);
    progress->SetPos(75);
    MyLog.print(Log::LOGD,"消息处理函数收到消息WM_GET_TEST_IMAGE");

    if(response.size()==0){
        MyLog.print(Log::LOGU,"接收背景数据超时");
    } else{
        saveImage(_T("collectedBGI"),response);
        progress->SetPos(100);
        MyLog.print(Log::LOGD,"加载图片完成");
        MyLog.print(Log::LOGU,"接收背景数据成功");
    }
}
