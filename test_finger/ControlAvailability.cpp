#include"stdafx.h"

#define foreach(it,container) for(auto it=container.begin();it!=container.end();it++)
#define Merge(Dest,Src) Dest.insert(Dest.end(),Src.begin(),Src.end())

typedef CtrlValidity::vec vec;

vec InitVec;
vec WorkedVec;
vec WorkingVec;

void CtrlValidity::InitCtrl(){
    InitVec=vec{cmbBaud,cmbWay,btnConnect,editAddress,editPassword};
    WorkedVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnEnroll,btnMatch,btnDeviceInfo,btnViewEnrollIds};
    WorkingVec=vec{btnCancel};
}

void CtrlValidity::Init(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Enable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void CtrlValidity::Connecting(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void CtrlValidity::BeforeConnect(vec except){
    Init(except);
}

void CtrlValidity::AfterConnect(vec except){
    Work(except);
}

void CtrlValidity::Work(vec except){
    vec Disable,Enable;
    Merge(Disable,InitVec);
    Merge(Disable,WorkingVec);
    Merge(Enable,WorkedVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void CtrlValidity::Working(vec except){
    vec Disable,Enable;
    Merge(Disable,InitVec);
    Merge(Disable,WorkedVec);
    Merge(Enable,WorkingVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

inline void CtrlValidity::update(vec& Disable,vec& Enable){
    foreach(it,Disable){
        disable(*it);
    }
    foreach(it,Enable){
        enable(*it);
    }
}

inline void CtrlValidity::disable(CWnd * pWnd){
    pWnd->EnableWindow(FALSE);
}

inline void CtrlValidity::enable(CWnd * pWnd){
    pWnd->EnableWindow(TRUE);
}
