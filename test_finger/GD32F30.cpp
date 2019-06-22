#include "stdafx.h"

// GD32's 获得原始图像监听器
__ILC(GD32F30,GetRawImage){
    setProgress(50);
    MyLog::trace("接收到数据包,大小为%d",response.size());
    MyLog::trace("线程向主线程发送消息WM_GET_RAW_IMAGE");
    sendMainDialogMessage(WM_GET_RAW_IMAGE);
    setProgress(75);

    if(response.size()==0){
        MyLog::user("接收数据超时");
    } else{
        saveImage(response);
        setProgress(100);
        MyLog::trace("加载图片完成");
        MyLog::user("接收数据成功");
    }
    flow.execVal();
}

// GD32's 获得测试图像监听器
__ILC(GD32F30,GetTestImage){
    setProgress(50);
    MyLog::trace("接收到数据包,大小为%d",response.size());
    MyLog::trace("线程向主线程发送消息WM_GET_TEST_IMAGE");
    sendMainDialogMessage(WM_GET_TEST_IMAGE);
    setProgress(75);

    if(response.size()==0){
        MyLog::user("接收背景数据超时");
    } else{
        saveBGImg(response);
        setProgress(100);
        MyLog::trace("加载图片完成");
        MyLog::user("接收背景数据成功");
    }
    flow.execVal();
}

// GD32's 读寄存器监听器
__ILC(GD32F30,ReadRegister){
    setProgress(50);
    MyLog::trace("接收到数据包,大小为%d",response.size());
    MyLog::trace("线程向主线程发送消息WM_READ_REGISTER");
    sendMainDialogMessage(WM_READ_REGISTER);
    setProgress(75);

    if(response.size()==0){
        MyLog::user("接收数据超时");
    } else{
        // 拿出response里的第一个字节,当做结果,赋值给EditReadRegVal
        setText(editReadRegVal,MyString::Format("%X",response.getPointer()[0]));
        MyLog::user("接收数据成功");
    }
    flow.execVal();
}

//GD32's 获得设备信息监听器
__ILC(GD32F30,DeviceInfo){
    /*
    [02] 44 30 30 2E 30 30 2E 30 31 00 [4D 61 79 20 32 37 20 32 30 31 39] 00 31 38 3A 33 30 3A 34 34 00
    PC Application log dialog display:
      Finger Chip Version: 03           (if 0xFF then directly display "Unknown Type")
      Firmware Version: D00.00.01       (un-available for parter/customer)
      Build Time: May 27 2019 15:48:56  (un-available for parter/customer)
    */
    // 芯片版本
    char ChipVersion;
    // 固件版本
    char* FirmwareVersion;
    // 编译日期
    char* BuildDate;
    // 编译时间
    char* BuildTime;
    // response的字节数组头
    uint8_t* ptr=response.getPointer();

    // 第一个字节为芯片版本 ChipVersion
    ChipVersion=*ptr;

    // 接下来一个字符串为固件版本 FirmwareVersion
    FirmwareVersion=(char*)++ptr;
    while(*ptr)ptr++;

    // 接下来一个字符串为编译日期 BuildDate
    BuildDate=(char*)++ptr;
    while(*ptr)ptr++;

    // 接下来一个字符串为编译时间 BuildTime
    BuildTime=(char*)++ptr;

    // 如果ChipVersion为0xFF(在char里为-1),就是未知版本
    if(ChipVersion==-1){
        MyLog::user("Finger Chip Version: Unknown Type");
    } else{
        MyLog::user("Finger Chip Version: %x",ChipVersion);
    }
    MyLog::user("Firmware Version: %s",FirmwareVersion);
    MyLog::user("Build Time: %s %s",BuildDate,BuildTime);

    flow.execVal();
}
