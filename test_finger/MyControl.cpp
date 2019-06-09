#include "stdafx.h"

MyString getText(CWnd * pWnd){
    static CString ret;
    pWnd->GetWindowText(ret);
    return ret;
}

void setText(CWnd * pWnd,MyString str){
    pWnd->SetWindowText(str);
}

int getInt(CWnd * pWnd){
    int ret;
    sscanf(getText(pWnd),"%d",&ret);
    return ret;
}

int getHex(CWnd * pWnd){
    int ret;
    sscanf(getText(pWnd),"%x",&ret);
    return ret;
}
