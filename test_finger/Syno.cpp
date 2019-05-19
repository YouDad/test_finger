#include "stdafx.h"

__ILC(Syno,GetImage){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://录入成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x02://传感器上无手指
            MyLog.debug("传感器上无手指");
            break;
        case 0x03://录入不成功或面积不足
            MyLog.debug("录入不成功或面积不足");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,UpImage){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            response.readData(1);
            SendMessage(hwnd,WM_GET_RAW_IMAGE,WM_GET_RAW_IMAGE,0);
            progress->SetPos(50);
            MyLog.print(Log::LOGD,"接收到数据包,大小为%d",response.readSize());
            MyLog.print(Log::LOGD,"线程向主线程发送消息WM_GET_RAW_IMAGE");
            progress->SetPos(75);
            MyLog.print(Log::LOGD,"消息处理函数收到消息WM_GET_RAW_IMAGE");

            saveImage(_T("collectedImage"),response);
            progress->SetPos(100);
            MyLog.print(Log::LOGD,"加载图片完成");
            MyLog.print(Log::LOGU,"接收数据成功");

            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x0f://不能发送后续数据包
            MyLog.debug("不能发送后续数据包");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,GenChar){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x06://图像太乱,生不成特征
            MyLog.print(Log::LOGE,"图像太乱,生不成特征");
            break;
        case 0x07://图像特征太少,生不成特征
            MyLog.print(Log::LOGE,"图像特征太少,生不成特征");
            break;
        case 0x15://图像缓冲区里没有图像,生不成图像
            MyLog.print(Log::LOGE,"图像缓冲区里没有图像,生不成图像");
            break;
        case 0x1b://寄存器设定内容错误
            MyLog.print(Log::LOGE,"寄存器设定内容错误");
            break;
        case 0x28://当前指纹模板与之前的模板之间有关联
            MyLog.print(Log::LOGU,"当前指纹模板与之前的模板之间有关联");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,GetEnrollImage){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x02://传感器上无手指
            MyLog.debug("传感器上无手指");
            break;
        case 0x03://录入不成功或面积不足
            MyLog.debug("录入不成功或面积不足");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,RegModel){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x0a://合并失败
            MyLog.debug("合并失败");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,StoreChar){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x0b://PageID超出指纹库范围
            MyLog.debug("PageID超出指纹库范围");
            break;
        case 0x18://写FLASH出错
            MyLog.debug("写FLASH出错");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,Search){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
        case 0x09://没搜索到
            MyLog.debug("没搜索到");
            break;
        case 0x17://手指没有移动
            MyLog.debug("手指没有移动");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,ReadIndexTable){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            response.readData(1);
            if(response.readSize()!=32){
                MyLog.error("ReadIndexTable命令执行失败");
            } else{
                MyString msg="已注册指纹编号为:";
                std::vector<int>v;
                uint8_t* p=response.getPointer();
                for(int i=0;i<32;i++){
                    if(p[i]){
                        for(int j=0;j<8;j++){
                            if(p[i]&(1<<j)){
                                v.push_back(i*8+j);
                            }
                        }
                    }
                }
                if(v.size()){
                    msg+=MyString::IntToMyString(v[0]);
                    for(int i=1;i<v.size();i++){
                        msg+=MyString(",")+MyString::IntToMyString(v[i]);
                    }
                }
                MyLog.user(msg);
            }
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            break;
    }
}
