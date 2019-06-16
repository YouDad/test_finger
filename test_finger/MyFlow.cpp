#include "stdafx.h"

// 当前流程
MyFlow flow;

// 尾插法添加一个流程,flowID如果不正确会报错
void MyFlow::add(int flowID,FlowFunction_t flow){
    // 检查flowID
    assert(flowID==this->flowVec.size());
    this->flowVec.push_back(flow);
}

// 执行第一个流程,参数是int
void MyFlow::start(int result){
    this->flowID=0;
    this->execVal(result);
}

// 执行最后一个流程,参数是int
void MyFlow::terminate(int result){
    this->flowID=this->flowVec.size()-1;
    this->execVal(result);
}

// 执行下一个流程,参数是result引用
void MyFlow::execRef(int & result){
    while(this->flowVec.size()>this->flowID&&this->flowID>=0){
        if(!this->flowVec[this->flowID](result)){
            break;
        }
    }
}

// 执行下一个流程,参数是result值
void MyFlow::execVal(int result){
    execRef(result);
}

// 清空流程
void MyFlow::clear(){
    this->flowID=0;
    this->flowVec.clear();
}

double MyFlow::percent(){
    return (this->flowID+1.0)/this->flowVec.size();
}

bool MyFlow::next(){
    return jump(this->flowID+1);
}

bool MyFlow::prev(){
    return jump(this->flowID-1);
}

bool MyFlow::jump(int flowID){
    if(this->flowVec.size()>flowID&&flowID>=0){
        this->flowID=flowID;
        return true;
    }
    return false;
}
