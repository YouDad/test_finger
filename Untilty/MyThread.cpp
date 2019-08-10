#include "MyThread.h"
#include <vector>
#include "MyLocker.h"

// 如果tmp为真,那么线程结束后会自动释放,所以只有new没有delete
MyThread::MyThread(ThreadFunction_t pFunction,bool tmp){
    pf=pFunction;
    temporary=tmp;
}

// 用于自我释放的结构体
struct temporaryParam{
    ThreadFunction_t pf;
    MyThread* beDelete;
};

// 需要自我释放的临时线程向量
std::vector<temporaryParam> TPVec;

// 用于停住主线程直到新线程获得size之后再继续
MyLocker getSize(0,1);

// 启动线程
bool MyThread::start(){
    if(temporary){
        // 看一下Vec里面是不是有空位置
        bool needPushBack=true;
        int posInVec;
        for(int i=0;i<TPVec.size();i++){
            if(TPVec[i].beDelete==0){
                TPVec[i].pf=pf;
                TPVec[i].beDelete=this;
                needPushBack=false;
                posInVec=i;
                break;
            }
        }
        // 没有空位置,push_back一个
        if(needPushBack){
            TPVec.push_back(temporaryParam{pf,this});
            posInVec=TPVec.size()-1;
        }
        // 开线程
        thread=CreateThread(0,0,temporaryRun,&posInVec,0,0);
        getSize.lock();
    } else{
        thread=CreateThread(0,0,run,&pf,0,0);

    }
    return thread!=0;
}

// 关闭线程
bool MyThread::terminate(){
    if(thread){
        TerminateThread(thread,-1);
        CloseHandle(thread);
        thread=0;
        return true;
    }
    return false;
}

// 返回线程是否运行
bool MyThread::isRun(){
    return thread!=0;
}

// 普通线程运行函数
DWORD WINAPI MyThread::run(LPVOID params){
    (*(ThreadFunction_t*)params)();
    return 0;
}

// 自释放线程运行函数
DWORD WINAPI MyThread::temporaryRun(LPVOID params){
    // 需要获得需要执行的临时线程在向量的下标
    int size=*(int*)params;
    getSize.unlock();
    TPVec[size].pf();
    delete TPVec[size].beDelete;
    TPVec[size].beDelete=0;
    return 0;
}
