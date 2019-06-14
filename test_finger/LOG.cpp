#include "stdafx.h"

// �߼�����'s �����е�ͼ��(AdjustingImage)
__ILC(LOG,AdvDbg_AdjImg){
    // ����������,����Ŀ�ͷ����������ĸ��������ͼ���ID
    conf["id"]=MyString((char*)response.getPointer(),response.size()-32*32);
    response.readData(response.size()-32*32);

    // Ҫ�Ŵ��õ�ԭʼͼ��
    uint8_t bigImg[64*64];
    imgSizeX2(response.getPointer(),32,32,bigImg);
    saveBmp(64,64,bigImg,"collectedTempImage",conf["AdvDbg_ImgId"]);

    // ��������ֱ��ͼ Histogram��ֱ��ͼ
    uint8_t Histogram[256*64];
    generateHistogram(Histogram,256,64,response.getPointer(),32,32);
    saveBmp(256,64,Histogram,"collectedTempImage",MyString("Histogram")+conf["AdvDbg_ImgId"]);

    // ��߼����Դ���׷��һ���ؼ�
    sendMainDialogMessage(WM_APPEND_CONTROLS);

    // ��������
    response.readData(32*32);
}

// Logһ����λ����Info
__ILC(LOG,Info){
    MyLog::debug((char*)response.getPointer());
}

clock_t clockTime;

// ��ʼ��ʱ
__ILC(LOG,MeasureTimeStart){
    clockTime=clock();
}

// ������ʱ
__ILC(LOG,MeasureTimeEnd){
    MyLog::debug("MeasureTime:%d",clock()-clockTime);
}