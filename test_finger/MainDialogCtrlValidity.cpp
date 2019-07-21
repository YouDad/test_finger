#include"stdafx.h"

// 抽取出来的重复代码,写成宏
#define Merge(Dest,Src) Dest.insert(Dest.end(),Src.begin(),Src.end())
#define MergePtr(Dest,Src) Dest.insert(Dest.end(),Src->begin(),Src->end())

// 重定义可以取消前缀,写着舒服
typedef MainDialogCtrlValidity::vec vec;

// 各种类型的控件向量
vec InitVec;
vec SynoVec;
vec WorkingVec;
vec WorkedVec;
vec GD32F30Vec;
vec Empty;

// 根据协议类型,返回准备工作向量
vec* getWorkedVec(){
    if(getProtocol()==SYNO){
        return &SynoVec;
    } else if(getProtocol()==GD32){
        return &GD32F30Vec;
    } else{
        return &Empty;
    }
}

// 初始化控件
void MainDialogCtrlValidity::InitCtrl(){
    InitVec=vec{cmbBaud,cmbWay,btnConnect,editPassword};
    // 协议对应按钮可用性在这里设置,如果在Vec里出现那对应协议就可以点
    SynoVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnContinueImage,btnEnroll,btnMatch,btnTest,
        btnViewEnrollIds,btnDeleteTemplate};
    GD32F30Vec=vec{cmbProtocolType,btnConnect,btnAdvDbg,btnTest,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnDeviceInfo};
    WorkedVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnEnroll,btnMatch,btnTest,
        btnDeviceInfo,btnViewEnrollIds,btnDeleteTemplate};
    WorkingVec=vec{btnCancel};
    Empty=vec{};
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,InitVec);
    update(Disable,Enable);
}

// 初始状态
void MainDialogCtrlValidity::Init(vec except){
    vec Disable,Enable;
    vec* GetWorkedVec=getWorkedVec();
    Merge(Disable,WorkedVec);
    MergePtr(Disable,GetWorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Enable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

// 连接中状态
void MainDialogCtrlValidity::Connecting(vec except){
    vec Disable,Enable;
    vec* GetWorkedVec=getWorkedVec();
    Merge(Disable,WorkedVec);
    MergePtr(Disable,GetWorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

// 在连接之前的状态
void MainDialogCtrlValidity::BeforeConnect(vec except){
    Init(except);
}

// 连接之后的状态
void MainDialogCtrlValidity::AfterConnect(vec except){
    Work(except);
}

// 准备态
void MainDialogCtrlValidity::Work(vec except){
    vec Disable,Enable;
    vec* GetWorkedVec=getWorkedVec();
    Merge(Disable,InitVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,WorkedVec);
    MergePtr(Enable,GetWorkedVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

// 工作态
void MainDialogCtrlValidity::Working(vec except){
    vec Disable,Enable;
    vec* GetWorkedVec=getWorkedVec();
    Merge(Disable,InitVec);
    Merge(Disable,WorkedVec);
    MergePtr(Disable,GetWorkedVec);
    Merge(Enable,WorkingVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

// 更新控件可用性
inline void MainDialogCtrlValidity::update(vec& Disable,vec& Enable){
    for(auto it=Disable.begin();it!=Disable.end();it++){
        disable(*it);
    }
    for(auto it=Enable.begin();it!=Enable.end();it++){
        enable(*it);
    }
}

// 禁用控件
inline void MainDialogCtrlValidity::disable(CWnd * pWnd){
    pWnd->EnableWindow(FALSE);
}

// 启用控件
inline void MainDialogCtrlValidity::enable(CWnd * pWnd){
    pWnd->EnableWindow(TRUE);
}
