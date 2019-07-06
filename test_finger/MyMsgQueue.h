#pragma once
#include "MyLocker.h"
#include "MyString.h"
#include <queue>

template<class T>
class MyMsgQueue{
public:
    void push(const T& Msg);
    T pop();
    MyMsgQueue(int maxMsgNum);
private:
    MyLocker locker;
    std::queue<T> q;
};

template<class T>
void MyMsgQueue<T>::push(const T& Msg){
    q.push(Msg);
    locker.unlock();
}

template<class T>
T MyMsgQueue<T>::pop(){
    locker.lock();
    T ret=q.front();
    q.pop();
    return ret;
}

template<class T>
inline MyMsgQueue<T>::MyMsgQueue(int maxMsgNum):locker(0,maxMsgNum){}
