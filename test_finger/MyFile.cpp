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

bool MyFile::OperateFile(MyString path,const char * mode,ReturnFileFunction_t f){
    FILE* fp;
    errno_t err=fopen_s(&fp,path,mode);
    if(err==ERROR_SUCCESS){
        bool ret=f(fp);
        fclose(fp);
        return ret;
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

bool MyFile::HaveCommands(MyString path){
    return false;
}

void fgetcUntil(FILE* fp,char* s,char delimeter){
    for(char*c=s;;c++){
        *c=fgetc(fp);
        if(delimeter==*c||EOF==*c){
            *c=0;
            break;
        }
    }
}

bool MyFile::ReadCommands(MyString path,MyString & TabName,std::vector<struct Command>& v){
    v.clear();
    return OperateFile(path,"r",
        (ReturnFileFunction_t)[&](FILE* fp)->bool{
            char ch=0;
            char key[1<<16],val[1<<16],section[1<<6];
            std::map<std::string,std::map<std::string,std::string>> m;
            // 开始解析
            while(ch!=EOF){
                ch=fgetc(fp);
                switch(ch){
                case '#':
                    while(fgetc(fp)!='\n');
                    break;
                case '[':
                    fgetcUntil(fp,section,']');
                    break;
                case EOF:
                case ' ':
                case '\t':
                case '\n':
                    break;
                default:
                    key[0]=ch;
                    fgetcUntil(fp,key+1,'=');
                    fgetcUntil(fp,val,'\n');
                    m[section][key]=val;
                }
            }
            // 防止为空
            if(m.count("Information")==0){
                return false;
            }
            TabName=m["Information"]["Name"];
            m.erase("Information");

            for(auto it=m.begin();it!=m.end();it++){
                // it->first是方括号的内容
                struct Command c;
                c.Name=it->first;
                MyString(it->second["CmdCode"]).Parse("%x",&c.CmdCode);
                MyString params=it->second["Params"];
                std::vector<MyString> strs;
                params.split(',',strs);
                for(auto jt=strs.begin();jt!=strs.end();jt++){
                    int a=-1,b=-1;
                    jt->Parse("%d",&a);
                    jt->Parse("%x",&b);
                    c.DfaultValues.push_back(max(a,b));
                }
                v.push_back(c);
            }
            return true;
        }
    );
}
