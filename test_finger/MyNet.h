#pragma once
#include "MyDefine.h"

// 获得在线版本
int NetGetVersion();
// 下载版本的函数,参数为对应版本和每次获得数据的回调函数
void NetDownload(int NetVersion,_Function_t(void,uint8_t* data,int size,int total) func);
// 获得版本详细说明
MyString NetVersionInfo(int NetVersion);
// 返回是否连接网络
bool isConnectedNet();