#include "stdafx.h"
#include "GET_TEST_IMAGE_Listener.h"

void GET_TEST_IMAGE_Listener::listen(DataPacket response){
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"Serial Thread:2接收到数据包,大小为%d",response.len);
    MyLog.print(Log::LOGD,"Serial Thread:3线程向主线程发送消息CMD_GET_TEST_IMAGE");
    SendMessage(hwnd,WM_GET_TEST_IMAGE,WM_GET_TEST_IMAGE,0);
    progress->SetPos(75);
    MyLog.print(Log::LOGD,"Main Thread:4消息处理函数收到消息WM_GET_TEST_IMAGE");

    if(response.len==0){
        MyLog.print(Log::LOGU,"接收背景数据超时");
    } else{
        saveImage(_T("collectedBGI"),response);
        progress->SetPos(100);
        MyLog.print(Log::LOGD,"Main Thread:5加载图片完成");
        MyLog.print(Log::LOGU,"接收背景数据成功");
    }
}
