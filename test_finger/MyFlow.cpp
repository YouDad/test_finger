#include "stdafx.h"
#include "MyFlow.h"

int FlowID=0;
std::vector<FlowFunction_t>Flow;

void ExecFlow(int& result){
    while(FlowID>=0&&FlowID<Flow.size()&&Flow[FlowID](result));
}
