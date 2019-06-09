#pragma once
#include "stdafx.h"

class CtrlValidity{
public:
    typedef std::vector<CWnd*> vec;
    static void InitCtrl();
    static void Init(vec except=vec());
    static void Connecting(vec except=vec());
    static void BeforeConnect(vec except=vec());
    static void AfterConnect(vec except=vec());
    static void Work(vec except=vec());
    static void Working(vec except=vec());
private:
    static void update(vec& dis,vec& en);
    //禁用控件
    static void disable(CWnd* pWnd);
    //启用控件
    static void enable(CWnd* pWnd);
};
