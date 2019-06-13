#include "stdafx.h"

__ILC(LOG,AdvDbg_AdjImg){
    conf["id"]=MyString((char*)response.getPointer(),response.size()-32*32);
    response.readData(response.size()-32*32);
    uint8_t bigImg[64*64];
    imgSizeX2(response.getPointer(),32,32,bigImg);
    saveBmp(64,64,bigImg,"collectedTempImage",conf["AdvDbg_ImgId"]);
    uint8_t Histogram[256*64];
    generateHistogram(Histogram,256,64,response.getPointer(),32,32);
    saveBmp(256,64,Histogram,"collectedTempImage",MyString("Histogram")+conf["AdvDbg_ImgId"]);
    sendMainDialogMessage(WM_APPEND_CONTROLS);
    response.readData(32*32);
}

__ILC(LOG,Info){
    MyLog::debug((char*)response.getPointer());
}

clock_t clockTime;

__ILC(LOG,MeasureTimeStart){
    clockTime=clock();
}

__ILC(LOG,MeasureTimeEnd){
    MyLog::debug("MeasureTime:%d",clock()-clockTime);
}
