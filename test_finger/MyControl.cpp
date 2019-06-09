#include "stdafx.h"

//返回控件的内容,类型是MyString
MyString getText(CWnd * pWnd){
    static CString ret;
    pWnd->GetWindowText(ret);
    return ret;
}

//设置控件的内容
void setText(CWnd * pWnd,MyString str){
    pWnd->SetWindowText(str);
}

//把控件中的内容当十进制解析返回一个整数
int getInt(CWnd * pWnd){
    int ret;
    getText(pWnd).Parse("%d",&ret);
    return ret;
}

//把控件中的内容当十六进制解析返回一个整数
int getHex(CWnd * pWnd){
    int ret;
    getText(pWnd).Parse("%x",&ret);
    return ret;
}

//将filePath的图像加载到图片控件上
void loadImage(CStatic* image,MyString filePath){
    HBITMAP hBmp=(HBITMAP)LoadImage(0,filePath,0,0,0,LR_LOADFROMFILE);
    if(hBmp){
        image->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
        image->SetBitmap(hBmp);
    } else{
        ASF_WARNING(4);
    }
}
