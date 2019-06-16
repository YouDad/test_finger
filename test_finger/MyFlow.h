#pragma once
#include "MyDefine.h"
#include <functional>

//返回true代表可以继续执行下一个流程,false就是执行之后中断
#define FlowFunction_t _Function_t(bool,int&)

class MyFlow{
public:
    // 尾插法添加一个流程,flowID如果不正确会报错
    void add(int flowID,FlowFunction_t flow);
    // 执行第一个流程,参数是int
    void start(int result=0);
    // 执行最后一个流程,参数是int
    void terminate(int result=0);
    // 执行下一个流程,参数是result引用
    void execRef(int& result);
    // 执行下一个流程,参数是result值
    void execVal(int result=0);
    // 清空流程
    void clear();
    // 返回流程百分比
    double percent();
    // 跳到下一个流程
    bool next();
    // 跳到上一个流程
    bool prev();
    // 跳到任意流程
    bool jump(int flowID);
private:
    // 当前流程的排列
    std::vector<FlowFunction_t> flowVec;
    // 当前流程的ID
    int flowID;
};

// 当前流程
extern MyFlow flow;
