#pragma once
#include"stdafx.h"

class MyLog{
public:
    enum LogLevel{
        LOGU,//用户级信息,给用户显示的信息
        LOGE,//错误级信息
        LOGW,//警告级信息
        LOGD,//调试级信息
        LOGT,//临时级信息
        LOG_HIGHEST//固有项,不要删
    };
    static void print(LogLevel level,MyString info);
    static void print(LogLevel level,const char* format,...);
    static void debug(MyString info);
    static void debug(const char* format,...);
    static void error(MyString info);
    static void error(const char* format,...);
    static void warn(MyString info);
    static void warn(const char* format,...);
    static void user(MyString info);
    static void user(const char* format,...);
    static void DevelopLog();
    static void ClearLog();
private:
    static const char* LOG_FILE;
    static bool isExistLog();
    static void clearLog();
    static void createLog();
    static void appendLog(const char * text);
};

extern int Version;
