#pragma once
#include "MyString.h"
#include "MyMsgQueue.h"

#define FastDeclearLogName(FuncName) \
static void FuncName(MyString info);\
static void FuncName(const char* format,...);

class MyLog{
public:
    enum LogLevel{
        LOGU,// 用户级信息,给用户显示的信息
        LOGE,// 错误级信息
        LOGW,// 警告级信息
        LOGD,// 调试级信息
        LOGT,// 临时级信息
        LOG_HIGHEST// 固有项,不要删
    };
    // 输出level的info
    static void print(LogLevel level,MyString info);
    // 输出level的info(printf的样子)
    static void print(LogLevel level,const char* format,...);
    FastDeclearLogName(trace)
    FastDeclearLogName(debug)
    FastDeclearLogName(error)
    FastDeclearLogName(warn)
    FastDeclearLogName(user)
    // 输出开发日志
    static void DevelopLog();
    // 追加日志内容
    static void appendLog(const char * text);
    // 消息队列
    static MyMsgQueue<MyString> MsgQueue;
};

// 版本号270=2.7,271=2.7.1
extern int Version;
