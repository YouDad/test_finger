#pragma once
#include"stdafx.h"

//枚举类型 动作,用于控制控件的可用性
enum action{
	actInit,        //初始化

	actOpeningPort,	//打开端口中
	actClosingPort,	//关闭端口中
	actOpenedPort,	//打开了端口
	actClosedPort,	//关闭了端口

	actGetingImage,	//获取图像中
	actGotImage,	//获取了图像
	actGetConImage,	//开始连续取图
	actStpGetImage,	//停止连续取图
	actGetConBKI,	//开始连续取图
	actStpGetBKI,	//停止连续取图
	actReadingReg,	//读寄存器中
	actReadedReg,	//读完了寄存器
	actWritingReg,	//写寄存器中
	actWritedReg	//写完了寄存器
};

//将CString类型变量变成char*型变量
//注意,由于用了static,用两次的话
//需要学这个函数,自己写两个转化
char* CString2char(CString&c);

//枚举串口 useful返回空闲的串口,used返回被占用的串口
void enumerateSerialPorts(CUIntArray* idle,CUIntArray* buzy);

//更新通信方式组合框
void updateCommunityWay();

//根据动作更新控件可用性
void updateControlDisable(action a);

//将filePath的图像加载到图片控件上
void loadImage(WCHAR* filePath);

bool saveBmp(int height,int width,BYTE*pData,CString dir,CString path);

void saveImage(CString x);