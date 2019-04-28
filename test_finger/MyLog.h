#pragma once
#include"stdafx.h"

class Log{
public:
    enum LogLevel{
        LOGU,//用户级信息,给用户显示的信息
        LOGE,//错误级信息
        LOGW,//警告级信息
        LOGD,//调试级信息
        LOGT,//临时级信息
        LOG_HIGHEST//固有项,不要删
    };
    const char* LOG_FILE="backup.txt";
    bool isExistLog();
    void clearLog();
    void createLog();
    void appendLog(wchar_t* text);
    void print(LogLevel level,MyString info);
    void print(LogLevel level,const char* format,...);
};

extern Log MyLog;
