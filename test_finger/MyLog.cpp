#pragma once
#include"stdafx.h"

const char* MyLog::LOG_FILE="backup_log.txt";

bool MyLog::isExistLog(){
    FILE* fp=fopen(LOG_FILE,"r");
    if(fp){
        fclose(fp);
    }
    return fp!=NULL;
}

void MyLog::clearLog(){
    if(isExistLog()){
        remove(LOG_FILE);
    }
}

void MyLog::createLog(){
    fclose(fopen(LOG_FILE,"a+"));
}

void MyLog::appendLog(const char* text){
    FILE* fp=fopen(LOG_FILE,"a+");
    fprintf_s(fp,"%s",text);
    fclose(fp);
}

void MyLog::print(LogLevel level,MyString info){
    //日志等级异常
    if(level>=LOG_HIGHEST){
        ASF_WARNING(1);
        level=LOGD;
    }
    //增加等级提示
    const char* pLevel;
    switch(level){
        case LOGU:pLevel=" U ";break;
        case LOGE:pLevel=" E ";break;
        case LOGW:pLevel=" W ";break;
        case LOGD:pLevel=" D ";break;
        case LOGT:pLevel=" T ";break;
        default:pLevel=" Unknown ";break;
    }
    //构造包装过后的信息
    MyString content=MyString::Time("%Y-%m-%d %H:%M:%S")+pLevel+info+"\r\n";
    //更新逻辑
    static MyString last_info="";
    if(info!=last_info){
        int len=editLog->GetWindowTextLength();
        MyString old_content=getText(editNow);
        editLog->SetSel(len,len,0);
        editLog->ReplaceSel(old_content);
        appendLog(old_content);
    }
    if(getText(editNow)!=content){
        setText(editNow,content);
    }
    last_info=info;
}

void MyLog::print(LogLevel level,const char* format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(level,MyString(tmp));
}

void MyLog::debug(MyString info){
    print(LOGD,info);
}

void MyLog::debug(const char * format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(LOGD,tmp);
}

void MyLog::error(MyString info){
    print(LOGE,info);
}

void MyLog::error(const char * format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(LOGE,tmp);
}

void MyLog::warn(MyString info){
    print(LOGW,info);
}

void MyLog::warn(const char * format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(LOGW,tmp);
}

void MyLog::user(MyString info){
    print(LOGU,info);
}

void MyLog::user(const char * format,...){
    static char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(LOGU,tmp);
}

int Version=260;
void MyLog::DevelopLog(){
    //print(MyLog::LOGU,"V0.9 <时间未知>:完成了串口连接和图片显示,完成了日志功能的建设");
    print(MyLog::LOGU,"V1.0 <2019年3月16日15:36:11>:完成原始图像和连续取图按钮功能");
    //print(MyLog::LOGU,"V1.1 <2019年3月16日15:54:23>:完成按钮互斥,防止线程冲突,添加开发日志");
    //print(MyLog::LOGU,"V1.2 <2019年3月18日00:57:44>:添加读写寄存器,添加进度条,添加选图像大小");
    //print(MyLog::LOGU,"V1.3 <2019年3月24日13:59:42>:完成了无用功能删减,放大了指纹图像,修复了按钮互斥bug");
    //print(MyLog::LOGU,"V1.4 <2019年3月24日14:12:08>:添加了在release模式下取消warning的代码,添加了打开文件夹按钮");
    //print(MyLog::LOGU,"V1.5 <2019年4月10日17:15:45>:完成连续取背景功能");
    //print(MyLog::LOGU,"V1.6 <2019年4月22日22:42:59>:适配了航芯的取原始图像功能,修复了时间和编辑框鬼畜bug");
    print(MyLog::LOGU,"V2.0 <2019年4月28日17:17:46>:升级了串口架构,支持主动和被动模式并存");
    //print(MyLog::LOGU,"V2.01<2019年4月29日12:15:13>:消除用于静电检测的竖线,修复了一些已知bug");
    print(MyLog::LOGU,"V2.1 <2019年4月29日16:55:54>:更新了包的确认方案,采用DataPacket来识别协议");
    print(MyLog::LOGU,"V2.2 <2019年4月30日14:44:30>:增加了高级调试功能");
    print(MyLog::LOGU,"V2.3 <2019年5月01日20:56:32>:上线新的协议ASFComm");
    //print(MyLog::LOGU,"V2.31<2019年5月01日23:12:42>:调整了AdjustingImage函数接口");
    print(MyLog::LOGU,"V2.4 <2019年5月20日01:19:35>:增加了Syno的协议,有注册/比对功能");
    print(MyLog::LOGU,"V2.5 <2019年5月20日22:37:36>:增加了Syno的删除/(连续)取图功能,增加更新功能");
    //print(MyLog::LOGU,"V2.51<2019年5月24日12:49:24>:修复GD32协议读寄存器bug,记忆波特率和协议");
    //print(MyLog::LOGU,"V2.52<2019年5月26日21:36:00>:增加GD32协议下的DeviceInfo命令,多一个设备信息的按钮,增强更新功能");
    print(MyLog::LOGU,"V2.6 <2019年5月27日22:49:28>:优化日志框性能,适配设备信息命令");
    print(MyLog::LOGU,"V2.61<2019年5月29日16:29:31>:修复设备信息命令ChipVersion bug");
}

void MyLog::ClearLog(){
    clearLog();
    createLog();
    setText(editLog,"");
}
