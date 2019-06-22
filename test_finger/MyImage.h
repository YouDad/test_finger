#pragma once
#include "MyString.h"

// 把dataPacket里的数据当做图片存为图片
void saveImage(DataPacket dataPacket);

// 把dataPacket里的数据当做图片存为背景图片
void saveBGImg(DataPacket dataPacket);

// 生成亮度直方图,输入为w*h的pData,输出为hw*hh的Histogram
void generateHistogram(uint8_t* Histogram,int hw,int hh,uint8_t* pData,int w,int h);

// 把w*h的src的图片放大一倍,存到dest里
void imgSizeX2(uint8_t* src,int w,int h,uint8_t* dest);

// 把w*h的pData图片存到id号临时图片中
bool saveTempImage(int w,int h,uint8_t* pData,int id);