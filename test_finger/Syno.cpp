#include "stdafx.h"

// 根据返回值生成Syno's 错误信息
const char* SynoFormatMessage(int errorCode){
    switch(errorCode){
        //case 0x00:return "指令执行完毕或OK";
    case 0x00:return "";
    case 0x01:return "数据包接收错误";
    case 0x02:return "传感器上没有手指";
    case 0x03:return "录入指纹图像失败或录入面积不足";
    case 0x04:return "指纹图像太干、太淡而生不成特征";
    case 0x05:return "指纹图像太湿、太糊而生不成特征";
    case 0x06:return "指纹图像太乱而生不成特征";
    case 0x07:return "指纹图像正常，但特征点太少（或面积太小）而生不成特征";
    case 0x08:return "指纹不匹配";
    case 0x09:return "没搜索到指纹";
    case 0x0a:return "特征合并失败";
    case 0x0b:return "访问指纹库时地址序号超出指纹库范围";
    case 0x0c:return "从指纹库读模板出错或无效";
    case 0x0d:return "上传特征失败";
    case 0x0e:return "模块不能接收后续数据包";
    case 0x0f:return "上传图像失败";
    case 0x10:return "删除模板失败";
    case 0x11:return "清空指纹库失败";
    case 0x12:return "不能进入低功耗状态";
    case 0x13:return "口令不正确";
    case 0x14:return "系统复位失败";
    case 0x15:return "缓冲区内没有有效原始图而生不成图像";
    case 0x16:return "在线升级失败";
    case 0x17:return "残留指纹或两次采集之间手指没有移动过";
    case 0x18:return "读写FLASH出错";
    case 0x19:return "随机数生成失败";
    case 0x1a:return "无效寄存器号";
    case 0x1b:return "寄存器设定内容错误号";
    case 0x1c:return "记事本页码指定错误";
    case 0x1d:return "端口操作失败";
    case 0x1e:return "自动注册（enroll）失败";
    case 0x1f:return "指纹库满";
    case 0x20:return "设备地址错误";
    case 0x21:return "密码有误";
    case 0x22:return "指纹模板非空";
    case 0x23:return "指纹模板为空";
    case 0x24:return "指纹库为空";
    case 0x25:return "录入次数设置错误";
    case 0x26:return "超时";
    case 0x27:return "指纹已存在";
    case 0x28:return "指纹模板有关联";
    case 0x29:return "传感器初始化失败";
    case 0x2A:return "模组唯一序列号非空";
    case 0x2B:return "模组唯一序列号为空";
    case 0x2C:return "OTP操作失败";
    case 0xf0:return "有后续数据包的指令，正确接收后用0xf0应答";
    case 0xf1:return "有后续数据包的指令，命令包用0xf1应答";
    case 0xf2:return "烧写内部FLASH时，校验和错误";
    case 0xf3:return "烧写内部FLASH时，包标识错误";
    case 0xf4:return "烧写内部FLASH时，包长度错误";
    case 0xf5:return "烧写内部FLASH时，代码长度太长";
    case 0xf6:return "烧写内部FLASH时，烧写FLASH失败";
    default:return "";
    }
}

// 对response的标准处理
// 即只对错误信息进行输出,然后执行下一个流程
void standardProcessResponse(DataPacket& response){
    int retVal=response.getPointer()[0];
    MyLog::user(SynoFormatMessage(retVal));
    flow.execRef(retVal);
}

// 从传感器上读入图像存于图像缓冲区
__ILC(Syno,GetImage){
    standardProcessResponse(response);
}

// 根据原始图像生成指纹特征存于特征文件缓冲区
__ILC(Syno,GenChar){
    standardProcessResponse(response);
}

// 精确比对特征文件缓冲区中的特征文件
__ILC(Syno,Match){
    int retVal=response.getPointer()[0];
    MyLog::user(SynoFormatMessage(retVal));
    if(retVal==0x00){// 成功
        response.readData(1);
        uint8_t* ptr=response.getPointer();
        MyLog::user("指纹匹配成功,得分:%d",ptr[0]*256+ptr[1]);
    }
    flow.execRef(retVal);
}

// 以特征文件缓冲区中的特征文件搜索整个或部分指纹库
__ILC(Syno,Search){
    standardProcessResponse(response);
}

// 将特征文件合并生成模板存于特征文件缓冲区
__ILC(Syno,RegModel){
    standardProcessResponse(response);
}

// 将特征缓冲区中的文件储存到flash指纹库中
__ILC(Syno,StoreChar){
    standardProcessResponse(response);
}

// 从flash指纹库中读取一个模板到特征缓冲区
__ILC(Syno,LoadChar){
    standardProcessResponse(response);
}

// 上传原始图像
__ILC(Syno,UpImage){
    int retVal=response.getPointer()[0];
    MyLog::user(SynoFormatMessage(retVal));
    if(retVal==0x00){// 成功
        response.readData(1);
        setProgress(50);
        MyLog::trace("接收到数据包,大小为%d",response.readSize());
        MyLog::trace("线程向主线程发送消息WM_GET_RAW_IMAGE");
        sendMainDialogMessage(WM_GET_RAW_IMAGE);
        setProgress(75);

        saveImage(response);
        setProgress(100);
        MyLog::trace("加载图片完成");
        MyLog::user("接收数据成功");
    }
    flow.execRef(retVal);
}

// 删除flash指纹库中的一个特征文件
__ILC(Syno,DeleteChar){
    standardProcessResponse(response);
}

// 读索引表
__ILC(Syno,ReadIndexTable){
    int retVal=response.getPointer()[0];
    MyLog::user(SynoFormatMessage(retVal));
    if(retVal==0x00){// 成功
        response.readData(1);
        // 数据长度一定是32个字节
        if(response.readSize()!=32){
            MyLog::user("读索引表失败");
        } else{
            // 构造已注册指纹编号序列
            MyString msg="已注册指纹编号为: ";
            uint8_t* p=response.getPointer();

            //给了32个uint8_t,每一位为1代表对应的指纹存在
            for(int i=0;i<32;i++){
                if(p[i]==0){
                    continue;
                }
                for(int j=0;j<8;j++){
                    if(p[i]&(1<<j)){
                        msg+=MyString::IntToMyString(i*8+j)+" ";
                    }
                }
            }
            MyLog::user(msg);
        }
    }
    flow.execRef(retVal);
}

// 注册用获取图像
__ILC(Syno,GetEnrollImage){
    standardProcessResponse(response);
}
