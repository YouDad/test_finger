#include "stdafx.h"

//当前流程的ID
int FlowID=0;
//当前流程的排列
std::vector<FlowFunction_t>Flow;

// 执行下一个流程,参数是int&
void ExecFlowRef(int& result){
    while(FlowID>=0&&FlowID<Flow.size()&&Flow[FlowID](result));
}

// 执行下一个流程,参数是int
void ExecFlowVal(int result){
    while(FlowID>=0&&FlowID<Flow.size()&&Flow[FlowID](result));
}

// 执行第一个流程,参数是int
void ExecStart(int result){
    FlowID=0;
    ExecFlowVal(result);
}

// 执行最后一个流程,参数是int
void ExecEnd(int result){
    FlowID=Flow.size()-1;
    ExecFlowVal(result);
}
