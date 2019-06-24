#include "stdafx.h"

// 高级调试's 调整中的图像(AdjustingImage)
__ILC(LOG,AdvDbg_AdjImg){
    // 存在配置里,命令的开头几个多余字母代表调整图像的ID
    conf["id"]=MyString((char*)response.getPointer(),response.size()-32*32);
    response.readData(response.size()-32*32);

    // 要放大获得的原始图像
    uint8_t bigImg[64*64];
    imgSizeX2(32,32,response.getPointer(),bigImg);
    saveTempImage(64,64,bigImg,0);

    // 生成亮度直方图 Histogram是直方图
    uint8_t Histogram[256*64];
    generateHistogram(32,32,response.getPointer(),256,64,Histogram);
    saveTempImage(64,64,bigImg,1);

    // 向高级调试窗口追加一批控件
    sendMainDialogMessage(WM_APPEND_CONTROLS);

    // 处理结束
    response.readData(32*32);
}

// Log一下下位机的Info
__ILC(LOG,Info){
    MyLog::debug((char*)response.getPointer());
}

clock_t clockTime;

// 开始计时
__ILC(LOG,MeasureTimeStart){
    clockTime=clock();
}

// 结束计时
__ILC(LOG,MeasureTimeEnd){
    MyLog::debug("MeasureTime:%d",clock()-clockTime);
}
