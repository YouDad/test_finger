#pragma once
#include "MyString.h"

// 把长为w,宽为h的图片pData,存到当前目录的dirname目录下,文件名叫filename
bool saveBmp(int w,int h,uint8_t* pData,MyString dirname,MyString filename);

// 把dataPacket里的数据当做图片存到dir路径下
void saveImage(MyString dir,DataPacket dataPacket);

// 生成亮度直方图,输入为w*h的pData,输出为hw*hh的Histogram
void generateHistogram(uint8_t* Histogram,int hw,int hh,uint8_t* pData,int w,int h);

// 把w*h的src的图片放大一倍,存到dest里
void imgSizeX2(uint8_t* src,int w,int h,uint8_t* dest);
