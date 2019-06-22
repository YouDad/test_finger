#pragma once

// 消息
#define WM_GET_RAW_IMAGE	WM_USER+5
#define WM_GET_CON_IMAGE	WM_USER+6
#define WM_STP_GET_IMAGE	WM_USER+7
#define WM_READ_REGISTER	WM_USER+8
#define WM_APPEND_CONTROLS	WM_USER+9
#define WM_GET_CON_BKI		WM_USER+10
#define WM_STP_GET_BKI		WM_USER+11
#define WM_GET_TEST_IMAGE	WM_USER+12

// 类型
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

// 宏定义
#define _Function_t(ReturnType,...) std::function<ReturnType(__VA_ARGS__)>
#define _Function(ReturnType,...) [&](__VA_ARGS__)->ReturnType

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Stringify(x) #x