#include"stdafx.h"

__ILC(ASFComm,GetRawImage){
    SendMessage(hwnd,WM_GET_RAW_IMAGE,WM_GET_RAW_IMAGE,0);
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"接收到数据包,大小为%d",response.size());
    MyLog.print(Log::LOGD,"线程向主线程发送消息WM_GET_RAW_IMAGE");
    progress->SetPos(75);
    MyLog.print(Log::LOGD,"消息处理函数收到消息WM_GET_RAW_IMAGE");

    if(response.size()==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        saveImage(_T("collectedImage"),response);
        progress->SetPos(100);
        MyLog.print(Log::LOGD,"加载图片完成");
        MyLog.print(Log::LOGU,"接收数据成功");
    }
}

__ILC(ASFComm,GetTestImage){
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

__ILC(ASFComm,ReadRegister){
    if(response.size()==0){
        MyLog.print(Log::LOGU,"接收数据超时");
    } else{
        setText(editReadRegVal,MyString::Format("%X",response.getPointer()[0]));
        MyLog.print(Log::LOGU,"接收数据成功");
    }
    sendMessage(WM_READ_REGISTER);
    updateControlDisable(actReadedReg);
}

__ILC(ASFComm,WriteRegister){
    //DoNothing
}

__ILC(ASFComm,ToSleep){
    //DoNothing
}

__ILC(ASFComm,ToIdle){
    //DoNothing
}

__ILC(ASFComm,Log){
    int level=MyString::ParseInt(MyString((char*)response.getPointer(),1));
    response.readData(1);
    MyLog.print((Log::LogLevel)level,(char*)response.getPointer());
}

__ILC(ASFComm,AdjustingImage){
    $::conf["id"]=MyString((char*)response.getPointer(),response.size()-32*32);
    response.readData(response.size()-32*32);
    BYTE bigImg[64*64];
    imgSizeX2(response.getPointer(),32,32,bigImg);
    saveBmp(64,64,bigImg,"collectedTempImage",$::conf["AdvDbg_ImgId"]);
    BYTE Histogram[256*64];
    generateHistogram(Histogram,256,64,response.getPointer(),32,32);
    saveBmp(256,64,Histogram,"collectedTempImage",MyString("Histogram")+$::conf["AdvDbg_ImgId"]);
    sendMessage(WM_APPEND_CONTROLS);
    response.readData(32*32);
}
