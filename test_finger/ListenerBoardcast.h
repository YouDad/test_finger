#pragma once
#include"stdafx.h"

// 监听器接口
class ICommListener{
public:
    // 构造器,参数是宏定义的字符串,代表这个监听器在什么协议下进行监听
    ICommListener(const char* protocol=0);
    // 如果可以监听,返回真
    bool accept();
    // 监听实际逻辑
    virtual void listen(DataPacket response)=0;
private:
    // 协议字符串
    const char* protocol;
};

// 监听器广播
class ListenerBoardcast{
public:
    // 构造器,初始化需要监听的map
    ListenerBoardcast();
    // 增加事件码对应的监听器
    void attach(int event,ICommListener*listener);
    // 广播事件
    void boardcast(int event,DataPacket response);
private:
    // 保存监听关系的map
    std::map<int,std::vector<ICommListener*>>m;
};

// 全局唯一监听器广播对象
extern ListenerBoardcast listenerBoardcast;
