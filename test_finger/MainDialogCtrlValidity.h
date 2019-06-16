#pragma once
#include "stdafx.h"

// MainDialog的控件可用性
class MainDialogCtrlValidity{
public:
    // 类型
    typedef std::vector<CWnd*> vec;
    // 初始化控件
    static void InitCtrl();
    // 初始状态
    static void Init(vec except=vec());
    // 连接中状态
    static void Connecting(vec except=vec());
    // 在连接之前的状态
    static void BeforeConnect(vec except=vec());
    // 连接之后的状态
    static void AfterConnect(vec except=vec());
    // 准备态
    static void Work(vec except=vec());
    // 工作态
    static void Working(vec except=vec());
private:
    // 更新控件可用性
    static void update(vec& dis,vec& en);
    // 禁用控件
    static void disable(CWnd* pWnd);
    // 启用控件
    static void enable(CWnd* pWnd);
};
