#include "stdafx.h"
#include "MyFlow.h"

int FlowID=0;
std::vector<FlowFunction_t>Flow;

void ExecFlowRef(int& result){
    while(FlowID>=0&&FlowID<Flow.size()&&Flow[FlowID](result));
}

void ExecFlowVal(int result){
    while(FlowID>=0&&FlowID<Flow.size()&&Flow[FlowID](result));
}

void ExecStart(int result){
    FlowID=0;
    ExecFlowVal(result);
}

void ExecEnd(int result){
    FlowID=Flow.size()-1;
    ExecFlowVal(result);
}
