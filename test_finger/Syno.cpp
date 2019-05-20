#include "stdafx.h"

__ILC(Syno,GetImage){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://录入成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            return;
        case 0x02://传感器上无手指
            MyLog.user("传感器上无手指");
            break;
        case 0x03://录入不成功或面积不足
            MyLog.user("录入不成功或面积不足");
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
            MyLog.debug("接收到数据包,大小为%d",response.readSize());
            MyLog.debug("线程向主线程发送消息WM_GET_RAW_IMAGE");
            progress->SetPos(75);
            MyLog.debug("消息处理函数收到消息WM_GET_RAW_IMAGE");

            saveImage(_T("collectedImage"),response);
            progress->SetPos(100);
            MyLog.debug("加载图片完成");
            MyLog.user("接收数据成功");

            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            return;
        case 0x0f://不能发送后续数据包
            MyLog.user("不能发送后续数据包");
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
            return;
        case 0x06://图像太乱,生不成特征
            MyLog.user("图像太乱,生不成特征");
            break;
        case 0x07://图像特征太少,生不成特征
            MyLog.user("图像特征太少,生不成特征");
            break;
        case 0x15://图像缓冲区里没有图像,生不成图像
            MyLog.user("图像缓冲区里没有图像,生不成图像");
            break;
        case 0x1b://寄存器设定内容错误
            MyLog.user("寄存器设定内容错误");
            break;
        case 0x28://当前指纹模板与之前的模板之间有关联
            MyLog.user("当前指纹模板与之前的模板之间有关联");
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
            return;
        case 0x02://传感器上无手指
            MyLog.user("传感器上无手指");
            break;
        case 0x03://录入不成功或面积不足
            MyLog.user("录入不成功或面积不足");
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
            return;
        case 0x0a://合并失败
            MyLog.user("合并失败");
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
            return;
        case 0x0b://PageID超出指纹库范围
            MyLog.user("PageID超出指纹库范围");
            break;
        case 0x18://写FLASH出错
            MyLog.user("写FLASH出错");
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
            return;
        case 0x09://没搜索到
            MyLog.user("没搜索到");
            break;
        case 0x17://手指没有移动
            MyLog.user("手指没有移动");
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
                MyLog.user("ReadIndexTable命令执行失败");
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
            return;
    }
    ExecFlow(front);
}

__ILC(Syno,Match){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            response.readData(1);
            MyLog.user("指纹匹配成功,得分:%d",response.getPointer()[0]*256+response.getPointer()[1]);
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            return;
        case 0x08://指纹不匹配
            MyLog.user("指纹不匹配");
            break;
    }
    ExecFlow(front);
}

__ILC(Syno,LoadChar){
    int front=response.getPointer()[0];
    switch(front){
        case 0x00://成功
            break;
        case 0x01://收包有错
            ASF_ERROR(5);
            return;
        case 0x0b://编号超出指纹库范围
            MyLog.user("编号超出指纹库范围");
            break;
        case 0x0c://读出有错或模板无效
            MyLog.user("读出有错或模板无效");
            break;
    }
    ExecFlow(front);
}
