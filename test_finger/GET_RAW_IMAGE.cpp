#pragma once
#include "stdafx.h"
#include "GET_RAW_IMAGE.h"

void GET_RAW_IMAGE_Listener::listen(DataPacket response){
    SendMessage(hwnd,WM_GET_RAW_IMAGE,WM_GET_RAW_IMAGE,0);
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"接收到数据包,大小为%d",response.len);
    MyLog.print(Log::LOGD,"线程向主线程发送消息WM_GET_RAW_IMAGE");
    progress->SetPos(75);
    MyLog.print(Log::LOGD,"消息处理函数收到消息WM_GET_RAW_IMAGE");

    if(response.len==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        saveImage(_T("collectedImage"),response);
        progress->SetPos(100);
        MyLog.print(Log::LOGD,"加载图片完成");
        MyLog.print(Log::LOGU,"接收数据成功");
    }
}
