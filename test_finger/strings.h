#pragma once

//WARNING STRING
#ifdef _DEBUG
#define ASF_WARNING(ID) MyLog::warn("%s at %s(%d)FuncName:<%s>",ASF_WARNING##ID,__FILE__,__LINE__,__FUNCTION__)
#else
#define ASF_WARNING(ID)
#endif
#define ASF_WARNING1 "好像调用了过于高级的MyLog::print"
#define ASF_WARNING2 "数据包格式不完全正确"
#define ASF_WARNING3 "图片像素个数不是160*160个"
#define ASF_WARNING4 "位图对象句柄为0x00"
#define ASF_WARNING5 "串口关闭返回值不是ERROR_SUCCESS"
#define ASF_WARNING__ ""
//ERROR STRING
#define ASF_ERROR(ID) 	MyLog::error("%s at %s(%d)FuncName:<%s>",ASF_ERROR##ID,__FILE__,__LINE__,__FUNCTION__)
#define ASF_ERROR1 "试图保存文件时失败"
#define ASF_ERROR2 "芯片类型错误"
#define ASF_ERROR3 "没有这种convert"
#define ASF_ERROR4 "转化失败"
#define ASF_ERROR5 "命令包出错"
#define ASF_ERROR6 "当前协议不支持该操作"
#define ASF_ERROR_ ""
//CONST STRING
#define SYNO "Syno"
#define GD32 "GD32F30"

#define NEW_INFO  MyLog::info("new   at %s(%d)<%s>",__FILE__,__LINE__,__FUNCTION__)
#define DEL_INFO  MyLog::info("del   at %s(%d)<%s>",__FILE__,__LINE__,__FUNCTION__)
#define NEWA_INFO MyLog::info("new[] at %s(%d)<%s>",__FILE__,__LINE__,__FUNCTION__)
#define DELA_INFO MyLog::info("del[] at %s(%d)<%s>",__FILE__,__LINE__,__FUNCTION__)