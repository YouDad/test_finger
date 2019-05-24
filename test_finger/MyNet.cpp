#include "stdafx.h"

#include <afxinet.h>

int NetGetVersion(){
    CInternetSession session;
    CHttpFile* pfile=(CHttpFile*)session.OpenURL(L"http://39.107.64.93/?Version=0");

    MyString content;
    DWORD dwStatusCode;
    pfile->QueryInfoStatusCode(dwStatusCode);
    if(dwStatusCode==HTTP_STATUS_OK){
        CString data;
        while(pfile->ReadString(data)){
            content+=(char*)data.GetBuffer();
        }
    }
    pfile->Close();
    delete pfile;
    session.Close();

    int BigVersion,SmlVersion;
    content.Parse("%d.%d",&BigVersion,&SmlVersion);
    if(SmlVersion<10){
        SmlVersion*=10;
    }
    return BigVersion*100+SmlVersion;
}

MyString description(int size){
    if(size>1024LL*1024*1024*8/7){
        return MyString::Format("%.2lfGB",1.0*size/1024/1024/1024);
    } else if(size>1024LL*1024*8/7){
        return MyString::Format("%.2lfMB",1.0*size/1024/1024);
    } else if(size>1024LL*8/7){
        return MyString::Format("%.2lfKB",1.0*size/1024);
    } else{
        return MyString::Format("%dB",size);
    }
}

void NetDownload(CProgressCtrl* process,CStatic* detail){
    if(2==MessageBoxA(0,"下载文件时不能操作该程序,即便如此也要下载么?",
                      "询问",MB_ICONEXCLAMATION|MB_OKCANCEL|MB_SYSTEMMODAL)){
        return;
    }
    CInternetSession session;
    int NetVersion=NetGetVersion();
    if(Version>NetVersion){
        if(2==MessageBoxA(0,"当前版本比远程版本高,即便如此也要下载么?",
                          "浪费流量警告",MB_ICONEXCLAMATION|MB_OKCANCEL|MB_SYSTEMMODAL)){
            return;
        }
    } else if(Version==NetVersion){
        MessageBoxA(0,"当前版本和远程版本一样","不能下载",MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
        return;
    }
    int BigVersion,SmlVersion;
    BigVersion=NetVersion/100;
    SmlVersion=NetVersion%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    int fileSize;
    CHttpFile* pfile=(CHttpFile*)session.OpenURL(MyString::Format(
        "http://39.107.64.93/?Version=%d.%d&VersionSize",BigVersion,SmlVersion));
    MyString size;
    DWORD dwStatusCode;
    pfile->QueryInfoStatusCode(dwStatusCode);
    if(dwStatusCode==HTTP_STATUS_OK){
        CString data;
        while(fileSize=pfile->ReadString(data)){
            size+=(char*)data.GetBuffer();
        }
        size.Parse("%d",&fileSize);
    }
    pfile->Close();
    delete pfile;

    pfile=(CHttpFile*)session.OpenURL(MyString::Format(
        "http://39.107.64.93/?Version=%d.%d",BigVersion,SmlVersion));
    FILE* fp=fopen(MyString::Format("test_fingerV%d.%d.release.exe",BigVersion,SmlVersion),"wb");
    pfile->QueryInfoStatusCode(dwStatusCode);
    int now=0;
    if(dwStatusCode==HTTP_STATUS_OK){
        uint8_t data[1024];
        int cnt=0;
        while((cnt=pfile->Read(data,1024))>0){
            fwrite(data,1,cnt,fp);
            now+=cnt;
            process->SetPos(100*now/fileSize);
            MyString nowStr=description(now);
            MyString fileSizeStr=description(fileSize);
            setText(detail,MyString::Format("%s/%s",(const char*)nowStr,(const char*)fileSizeStr));
        }
    }
    MessageBoxA(0,"下载完毕!","OK",MB_OK);
    pfile->Close();
    delete pfile;
    session.Close();
    fclose(fp);
}

bool isConnectedNet(){
    DWORD flags;
    return InternetGetConnectedState(&flags,0);
}
