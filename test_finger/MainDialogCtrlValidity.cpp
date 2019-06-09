#include"stdafx.h"

#define foreach(it,container) for(auto it=container.begin();it!=container.end();it++)
#define Merge(Dest,Src) Dest.insert(Dest.end(),Src.begin(),Src.end())

typedef MainDialogCtrlValidity::vec vec;

vec InitVec;
vec WorkedVec;
vec WorkingVec;

void MainDialogCtrlValidity::InitCtrl(){
    InitVec=vec{cmbBaud,cmbWay,btnConnect,editAddress,editPassword};
    WorkedVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnEnroll,btnMatch,btnDeviceInfo,btnViewEnrollIds};
    WorkingVec=vec{btnCancel};
}

void MainDialogCtrlValidity::Init(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Enable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::Connecting(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::BeforeConnect(vec except){
    Init(except);
}

void MainDialogCtrlValidity::AfterConnect(vec except){
    Work(except);
}

void MainDialogCtrlValidity::Work(vec except){
    vec Disable,Enable;
    Merge(Disable,InitVec);
    Merge(Disable,WorkingVec);
    Merge(Enable,WorkedVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::Working(vec except){
    vec Disable,Enable;
    Merge(Disable,InitVec);
    Merge(Disable,WorkedVec);
    Merge(Enable,WorkingVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

inline void MainDialogCtrlValidity::update(vec& Disable,vec& Enable){
    foreach(it,Disable){
        disable(*it);
    }
    foreach(it,Enable){
        enable(*it);
    }
}

inline void MainDialogCtrlValidity::disable(CWnd * pWnd){
    pWnd->EnableWindow(FALSE);
}

inline void MainDialogCtrlValidity::enable(CWnd * pWnd){
    pWnd->EnableWindow(TRUE);
}
