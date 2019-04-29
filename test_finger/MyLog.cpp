#pragma once
#include"stdafx.h"
#include "MyLog.h"

Log MyLog;
bool Log::isExistLog(){
    FILE* fp=fopen(LOG_FILE,"r");
    if(fp){
        fclose(fp);
    }
    return fp!=NULL;
}

void Log::clearLog(){
    if(isExistLog()){
        remove(LOG_FILE);
    }
}

void Log::createLog(){
    fclose(fopen(LOG_FILE,"a+"));
}

void Log::appendLog(wchar_t* text){
    FILE* fp=fopen(LOG_FILE,"a+");
    fwprintf_s(fp,_T("%s"),text);
    fclose(fp);
}

void Log::print(LogLevel level,MyString info){
    if(level>=LOG_HIGHEST){
        print(Log::LOGW,"%s at %s(%d)FuncName:<%s>",ASF_WARNING1,__FILE__,__LINE__,__FUNCTION__);
    }
    if(level>cmbLogLevel->GetCurSel())return;

    time_t curtime;
    time(&curtime);
    MyString time(ctime(&curtime));
    MyString content=time+" "+info+"\r\n";
    int len=editLog->GetWindowTextLength();
    //选定当前文本的末端
    editLog->SetSel(len,len);
    //在编辑框中追加文本
    editLog->ReplaceSel(content);
    //在文件中追加文本
    appendLog(content);
}

void Log::print(LogLevel level,const char* format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(level,MyString(tmp));
}

void Log::debug(MyString info){
    print(LOGD,info);
}

void Log::debug(const char * format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(LOGD,tmp);
}

void Log::DevelopLog(){
    print(Log::LOGU,"V0.9 <时间未知>:完成了串口连接和图片显示,完成了日志功能的建设");
    print(Log::LOGU,"V1.0 <2019年3月16日15:36:11>:完成原始图像和连续取图按钮功能");
    print(Log::LOGU,"V1.1 <2019年3月16日15:54:23>:完成按钮互斥,防止线程冲突,添加开发日志");
    print(Log::LOGU,"V1.2 <2019年3月18日00:57:44>:添加读写寄存器,添加进度条,添加选图像大小");
    print(Log::LOGU,"V1.3 <2019年3月24日13:59:42>:完成了无用功能删减,放大了指纹图像,修复了按钮互斥bug");
    print(Log::LOGU,"V1.4 <2019年3月24日14:12:08>:添加了在release模式下取消warning的代码,添加了打开文件夹按钮");
    print(Log::LOGU,"V1.5 <2019年4月10日17:15:45>:完成连续取背景功能");
    print(Log::LOGU,"V1.6 <2019年4月22日22:42:59>:适配了航芯的取原始图像功能,修复了时间和编辑框鬼畜bug");
    print(Log::LOGU,"V2.0 <2019年4月28日17:17:46>:升级了串口架构,支持主动和被动模式并存");
    print(Log::LOGU,"V2.01<2019年4月29日12:15:13>:消除用于静电检测的竖线,修复了一些已知bug");
}
