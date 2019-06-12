#include"stdafx.h"

#define foreach(it,container) for(auto it=container.begin();it!=container.end();it++)
#define Merge(Dest,Src) Dest.insert(Dest.end(),Src.begin(),Src.end())
#define MergePtr(Dest,Src) Dest.insert(Dest.end(),Src->begin(),Src->end())

typedef MainDialogCtrlValidity::vec vec;

vec InitVec;
vec SynoVec;
vec WorkingVec;
vec WorkedVec;
vec GD32F30Vec;
vec Empty;

vec* getWorkedVec(){
    if(getText(cmbProtocolType)=="Syno"){
        return &SynoVec;
    } else if(getText(cmbProtocolType)=="GD32F30"){
        return &GD32F30Vec;
    } else{
        return &Empty;
    }
}

void MainDialogCtrlValidity::InitCtrl(){
    InitVec=vec{cmbBaud,cmbWay,btnConnect,editAddress,editPassword};
    //协议对应按钮可用性在这里设置,如果在Vec里出现那对应协议就可以点
    SynoVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnContinueImage,btnEnroll,btnMatch,
        btnViewEnrollIds,btnDeleteTemplate};
    GD32F30Vec=vec{cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnDeviceInfo};
    WorkedVec=vec{editFingerId,cmbProtocolType,btnConnect,btnAdvDbg,
        btnRawImage,btnTestImage,btnContinueImage,btnContinueBGImg,
        btnReadReg,btnWriteReg,btnEnroll,btnMatch,
        btnDeviceInfo,btnViewEnrollIds,btnDeleteTemplate};
    WorkingVec=vec{btnCancel};
    Empty=vec{};
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    Merge(Disable,WorkingVec);
    Merge(Disable,InitVec);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::Init(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    MergePtr(Disable,getWorkedVec());
    Merge(Disable,WorkingVec);
    Merge(Enable,InitVec);
    Merge(Enable,except);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::Connecting(vec except){
    vec Disable,Enable;
    Merge(Disable,WorkedVec);
    MergePtr(Disable,getWorkedVec());
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
    Merge(Disable,WorkedVec);
    MergePtr(Enable,getWorkedVec());
    Merge(Enable,except);
    update(Disable,Enable);
}

void MainDialogCtrlValidity::Working(vec except){
    vec Disable,Enable;
    Merge(Disable,InitVec);
    Merge(Disable,WorkedVec);
    MergePtr(Disable,getWorkedVec());
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
