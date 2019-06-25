#include "stdafx.h"

MyString MyFile::DATA_DIR="%temp%\\ixv\\";
MyString MyFile::LOG_DIR="log\\";
MyString MyFile::IMAGE_DIR="image\\";
MyString MyFile::BACKGROUND_DIR="bgimg\\";
MyString MyFile::HISTOGRAM_DIR="histogram\\";
MyString MyFile::TEMP_IMAGE_PATH="tempImage\\";
MyString MyFile::CONF_PATH="config";
MyString MyFile::RUN_TIME=init();

// 检查目录,顺便返回时间
MyString MyFile::init(){
    WCHAR TEMP_PATH[MAX_PATH];
    GetTempPath(MAX_PATH,TEMP_PATH);
    DATA_DIR=TEMP_PATH;
    DATA_DIR+="ixv\\";

    LOG_DIR=DATA_DIR+LOG_DIR;
    IMAGE_DIR=DATA_DIR+IMAGE_DIR;
    BACKGROUND_DIR=DATA_DIR+BACKGROUND_DIR;
    HISTOGRAM_DIR=DATA_DIR+HISTOGRAM_DIR;
    TEMP_IMAGE_PATH=DATA_DIR+TEMP_IMAGE_PATH;
    CONF_PATH=DATA_DIR+CONF_PATH;

    if(0!=_access(DATA_DIR,0)){
        int x=CreateDirectory(DATA_DIR,0);
        int d=GetLastError();
        int y=0;
    }
    if(0!=_access(LOG_DIR,0)){
        CreateDirectory(LOG_DIR,0);
    }
    if(0!=_access(IMAGE_DIR,0)){
        CreateDirectory(IMAGE_DIR,0);
    }
    if(0!=_access(BACKGROUND_DIR,0)){
        CreateDirectory(BACKGROUND_DIR,0);
    }
    if(0!=_access(HISTOGRAM_DIR,0)){
        CreateDirectory(HISTOGRAM_DIR,0);
    }
    if(0!=_access(TEMP_IMAGE_PATH,0)){
        CreateDirectory(TEMP_IMAGE_PATH,0);
    }
    return MyString::Time();
}

bool MyFile::OperateFile(MyString path,const char * mode,FileFunction_t f){
    FILE* fp;
    errno_t err=fopen_s(&fp,path,mode);
    if(err==ERROR_SUCCESS){
        f(fp);
        fclose(fp);
        return true;
    } else{
        char buffer[1024];
        strerror_s(buffer,err);
        MyLog::error(buffer);
    }
    return false;
}

bool MyFile::ReadConfig(FileFunction_t f){
    return OperateFile(CONF_PATH,"r",f);
}

bool MyFile::WriteConfig(FileFunction_t f){
    return OperateFile(CONF_PATH,"w",f);
}

bool MyFile::DownloadUpdate(int version,FileFunction_t f){
    // 准备下载
    int BigVersion,SmlVersion;
    BigVersion=version/100;
    SmlVersion=version%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    MyString fileName="test_fingerV";
    fileName+=MyString::Format("%d.%d",BigVersion,SmlVersion);
    fileName+=".release.exe";
    return OperateFile(fileName,"wb",f);
}

bool MyFile::AppendLog(FileFunction_t f){
    return OperateFile(LOG_DIR+"BackupLog_"+RUN_TIME+".txt","a+",f);
}

bool MyFile::LogSaveAs(FileFunction_t f){
    // 获取文件路径名
    LPCTSTR filter=_T("文本文件(*.txt)|*.txt||");
    CFileDialog dlgFileOpen(0,0,0,0,filter,0);
    if(dlgFileOpen.DoModal()==IDOK){
        MyString path=dlgFileOpen.GetPathName();
        //添加后缀 .txt
        if(path.find(".txt",path.length()-4)==-1){
            path=path+".txt";
        }
        //写到文件中
        return OperateFile(path,"w",f);
    }
    return false;
}

bool MyFile::SaveImage(MyString fileName,FileFunction_t f){
    return OperateFile(IMAGE_DIR+fileName,"wb",f);
}

bool MyFile::SaveBGImg(MyString fileName,FileFunction_t f){
    return OperateFile(BACKGROUND_DIR+fileName,"wb",f);
}

bool MyFile::SaveTempImage(FileFunction_t f,int id){
    return OperateFile(TEMP_IMAGE_PATH+MyString::IntToMyString(id)+".bmp","wb",f);
}
