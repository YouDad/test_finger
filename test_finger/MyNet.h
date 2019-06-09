#pragma once
#include "MyDefine.h"

int NetGetVersion();
void NetDownload(int NetVersion,_Function_t(void,uint8_t* data,int size,int total) func);
MyString NetVersionInfo(int NetVersion);
bool isConnectedNet();