#pragma once
#include "MyDefine.h"

//返回true代表可以继续执行下一个流程,false就是执行之后中断
#define FlowFunction_t _Function_t(bool,int&)
#define __FlowFunction(arg) _Function(bool,arg)
//返回true代表可以继续执行下一个流程,false就是执行之后中断,id指明该流程的ID
#define FlowFunction(id) __FlowFunction
//当前流程的ID
extern int FlowID;
//当前流程的排列
extern std::vector<FlowFunction_t> Flow;

void ExecFlowRef(int& result);
void ExecFlowVal(int result=0);
void ExecStart(int result=0);
void ExecEnd(int result=0);