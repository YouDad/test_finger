#include "stdafx.h"

// 返回控件的内容,类型是MyString
MyString getText(CWnd * pWnd){
    if(test.isTest(test.My)){
        throw 0;
    }
    static CString ret;
    pWnd->GetWindowText(ret);
    return ret;
}

// 设置控件的内容
void setText(CWnd * pWnd,MyString str){
    if(test.isTest(test.My)){
        test.myTest.My_CtrlSet.push_back(std::make_pair(pWnd,str));
        return;
    }
    pWnd->SetWindowText(str);
}

// 设置选项框的勾选状态
void setCheck(CButton * pCheck,bool select){
    pCheck->SetCheck(select?TRUE:FALSE);
}

// 获得选项框的勾选状态
bool isChecked(CButton * pCheck){
    return pCheck->GetCheck()==BST_CHECKED;
}

// 把控件中的内容当十进制解析返回一个整数
int getInt(CWnd * pWnd){
    int ret;
    getText(pWnd).Parse("%d",&ret);
    return ret;
}

// 把控件中的内容当十六进制解析返回一个整数
int getHex(CWnd * pWnd){
    int ret;
    getText(pWnd).Parse("%x",&ret);
    return ret;
}

// 将filePath的图像加载到图片控件上
void loadImage(CStatic* image,MyString filePath){
    if(test.isTest(test.UI)){
        return;
    }
    HBITMAP hBmp=(HBITMAP)LoadImage(0,filePath,0,0,0,LR_LOADFROMFILE);
    if(hBmp){
        image->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
        image->SetBitmap(hBmp);
    } else{
        ASF_WARNING(4);
    }
}

// 获得所选协议
MyString getProtocol(){
    if(test.isTest(test.UI)){
        return MyString(test.uiTest.UI_protocol);
    }
    return getText(cmbProtocolType);
}
