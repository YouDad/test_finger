#include "stdafx.h"

MyString MyLog::content;
MyLocker MyLog::contentlocker=MyLocker(0,1);

// 追加日志内容
void MyLog::appendLog(const char* text){
    if(conf["AutoLog"]==Stringify(true)){
        MyFile::AppendLog(
            [&](FILE* fp){
                fprintf_s(fp,"%s",text);
            }
        );
    }
}

// 输出level的info
void MyLog::print(LogLevel level,MyString info){
    // 日志等级异常
    if(level>=LOG_HIGHEST){
        ASF_WARNING(1);
        level=LOGD;
    }

    // 空信息不输出
    if(info==""){
        return;
    }

    if(test.isTest(test.My)){
        test.myTest.My_logs.push_back(info);
        return;
    }

    // 增加等级提示
    MyString pLevel;
    switch(level){
    case LOGU:pLevel="U";break;
    case LOGE:pLevel="E";break;
    case LOGW:pLevel="W";break;
    case LOGD:pLevel="D";break;
    case LOGT:pLevel="T";break;
    default:pLevel=" Unknown ";break;
    }

    // 构造包装过后的信息
    content=pLevel+MyString::Time("%Y-%m-%d %H:%M:%S ")+info+"\r\n";
    // 更新日志框
    static MyString last_info="";
    if(info!=last_info){
        sendMainDialogMessage(WM_ADDLOG);
    }
    sendMainDialogMessage(WM_LOG);
    contentlocker.lock();
    last_info=info;
}

// 输出level的info(printf的样子)
void MyLog::print(LogLevel level,const char* format,...){
    char tmp[512];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);

    print(level,MyString(tmp));
}

// 快速定义各个等级的log函数
#define FastDefineLogName(FuncName,LogLevel)    \
void MyLog::FuncName(MyString info){            \
    print(LogLevel,info);                       \
}                                               \
void MyLog::FuncName(const char * format,...){  \
    char tmp[512];                              \
    va_list ap;                                 \
    va_start(ap,format);                        \
    vsprintf(tmp,format,ap);                    \
    va_end(ap);                                 \
                                                \
    print(LogLevel,tmp);                        \
}

FastDefineLogName(trace,LOGT)
FastDefineLogName(debug,LOGD)
FastDefineLogName(warn,LOGW)
FastDefineLogName(error,LOGE)
FastDefineLogName(user,LOGU)

// 当前版本
int Version=280;
// 开发日志,非最新大版本的小版本和非最新级大版本加以注释
void MyLog::DevelopLog(){
    //user("V0.9 <时间未知>:完成了串口连接和图片显示,完成了日志功能的建设");
    user("V1.0 <2019年3月16日15:36:11>:完成原始图像和连续取图按钮功能");
    //user("V1.1 <2019年3月16日15:54:23>:完成按钮互斥,防止线程冲突,添加开发日志");
    //user("V1.2 <2019年3月18日00:57:44>:添加读写寄存器,添加进度条,添加选图像大小");
    //user("V1.3 <2019年3月24日13:59:42>:完成了无用功能删减,放大了指纹图像,修复了按钮互斥bug");
    //user("V1.4 <2019年3月24日14:12:08>:添加了在release模式下取消warning的代码,添加了打开文件夹按钮");
    //user("V1.5 <2019年4月10日17:15:45>:完成连续取背景功能");
    //user("V1.6 <2019年4月22日22:42:59>:适配了航芯的取原始图像功能,修复了时间和编辑框鬼畜bug");
    user("V2.0 <2019年4月28日17:17:46>:升级了串口架构,支持主动和被动模式并存");
    //user("V2.01<2019年4月29日12:15:13>:消除用于静电检测的竖线,修复了一些已知bug");
    user("V2.1 <2019年4月29日16:55:54>:更新了包的确认方案,采用DataPacket来识别协议");
    user("V2.2 <2019年4月30日14:44:30>:增加了高级调试功能");
    user("V2.3 <2019年5月01日20:56:32>:上线新的协议ASFComm");
    //user("V2.31<2019年5月01日23:12:42>:调整了AdjustingImage函数接口");
    user("V2.4 <2019年5月20日01:19:35>:增加了Syno的协议,有注册/比对功能");
    user("V2.5 <2019年5月20日22:37:36>:增加了Syno的删除/(连续)取图功能,增加更新功能");
    //user("V2.51<2019年5月24日12:49:24>:修复GD32协议读寄存器bug,记忆波特率和协议");
    //user("V2.52<2019年5月26日21:36:00>:增加GD32协议下的DeviceInfo命令,多一个设备信息的按钮,增强更新功能");
    user("V2.6 <2019年5月27日22:49:28>:优化日志框性能,适配设备信息命令");
    //user("V2.61<2019年5月29日16:29:31>:修复设备信息命令ChipVersion bug");
    user("V2.7 <2019年6月12日20:25:27>:增加USB连接方式,增强串口扫描功能,增强界面健壮性,优化代码结构");
    user("V2.8 <2019年6月25日09:09:29>:把配置剥离了出来,让用户可以配置.增加图像调整大小的功能");
}
