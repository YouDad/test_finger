#pragma once
#include "MyString.h"
#include <afxwin.h>

// 返回控件的内容,类型是MyString
MyString getText(CWnd* pWnd);

// 设置控件的内容
void setText(CWnd* pWnd,MyString str);

// 把控件中的内容当十进制解析返回一个整数
int getInt(CWnd* pWnd);

// 把控件中的内容当十六进制解析返回一个整数
int getHex(CWnd* pWnd);

// 将filePath的图像加载到图片控件上
void loadImage(CStatic* image,MyString filePath);

// 获得所选协议
MyString getProtocol();
