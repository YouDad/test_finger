#include "stdafx.h"

void READ_REGISTER_Listener::listen(DataPacket response){
    if(response.size()==0){
        MyLog::user("接收数据超时");
    } else{
        setText(editReadRegVal,MyString::Format("%X",response.getPointer()[0]));
        MyLog::user("接收数据成功");
    }
    sendMessage(WM_READ_REGISTER);
    CtrlValidity::Work();
}

void GET_TEST_IMAGE_Listener::listen(DataPacket response){
    progress->SetPos(50);
    MyLog::debug("接收到数据包,大小为%d",response.size());
    MyLog::debug("线程向主线程发送消息CMD_GET_TEST_IMAGE");
    sendMessage(WM_GET_TEST_IMAGE);
    progress->SetPos(75);
    MyLog::debug("消息处理函数收到消息WM_GET_TEST_IMAGE");

    if(response.size()==0){
        MyLog::user("接收背景数据超时");
    } else{
        saveImage(_T("collectedBGI"),response);
        progress->SetPos(100);
        MyLog::debug("加载图片完成");
        MyLog::user("接收背景数据成功");
    }
}

void GET_RAW_IMAGE_Listener::listen(DataPacket response){
    SendMessage(hwnd,WM_GET_RAW_IMAGE,WM_GET_RAW_IMAGE,0);
    progress->SetPos(50);
    MyLog::debug("接收到数据包,大小为%d",response.size());
    MyLog::debug("线程向主线程发送消息WM_GET_RAW_IMAGE");
    progress->SetPos(75);
    MyLog::debug("消息处理函数收到消息WM_GET_RAW_IMAGE");

    if(response.size()==0){
        MyLog::user("接收数据超时");
    } else{
        saveImage(_T("collectedImage"),response);
        progress->SetPos(100);
        MyLog::debug("加载图片完成");
        MyLog::user("接收数据成功");
    }
}
