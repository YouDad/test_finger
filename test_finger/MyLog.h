#pragma once
#include "MyString.h"

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
    // LOGT的print
    static void trace(MyString info);
    // LOGT的print
    static void trace(const char* format,...);
    // LOGD的print
    static void debug(MyString info);
    // LOGD的print
    static void debug(const char* format,...);
    // LOGE的print
    static void error(MyString info);
    // LOGE的print
    static void error(const char* format,...);
    // LOGW的print
    static void warn(MyString info);
    // LOGW的print
    static void warn(const char* format,...);
    // LOGU的print
    static void user(MyString info);
    // LOGU的print
    static void user(const char* format,...);
    // 输出开发日志
    static void DevelopLog();
    // 清空日志框
    static void ClearLog();
private:
    // 日志备份位置
    static const char* LOG_FILE;
    // 是否已经存在日志文件
    static bool isExistLog();
    // 删除日志文件
    static void clearLog();
    // 创造日志文件
    static void createLog();
    // 追加日志内容
    static void appendLog(const char * text);
};

// 版本号270=2.7,271=2.7.1
extern int Version;
