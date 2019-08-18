#include "stdafx.h"

MyString MyFile::DATA_DIR="%temp%\\ixv\\";
MyString MyFile::LOG_DIR="log\\";
MyString MyFile::IMAGE_DIR="image\\";
MyString MyFile::BACKGROUND_DIR="bgimg\\";
MyString MyFile::HISTOGRAM_DIR="histogram\\";
MyString MyFile::TEMP_IMAGE_PATH="tempImage\\";
MyString MyFile::CHAR_DIR="char\\";
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
    CHAR_DIR=DATA_DIR+CHAR_DIR;
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
    if(0!=_access(CHAR_DIR,0)){
        CreateDirectory(CHAR_DIR,0);
    }
    return MyString::Time();
}

bool MyFile::OperateFile(MyString path,const char * mode,FileFunction_t f){
    FILE* fp;
    errno_t err=fopen_s(&fp,path,mode);
    if(err==ERROR_SUCCESS&&fp!=0){
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

bool MyFile::Read(MyString fileName,ReturnFileFunction_t f){
    return OperateFile(fileName,"r",f);
}

bool MyFile::Write(MyString fileName,ReturnFileFunction_t f){
    return OperateFile(fileName,"w",f);
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
            char* key=new char[1<<16];
            char* val=new char[1<<16];
            char section[1<<6]={};
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
                    c.DefaultValues.push_back(max(a,b));
                }
                v.push_back(c);
            }
            delete[] key;
            delete[] val;
            return true;
        }
    );
}

bool MyFile::SaveDefaultPlugin1(MyString path){
    char* str1="# 这个文件必须是ANSI格式的\n\
# 井号用来注释\n\
# 但是只能注释一行,不能行内注释,会解释出错\n\
# Information节必须存在,而且必须有Name键值\n\
# 每个命令节必须有CmdCode,而且值是一个十六进制表示的数字\n\
# 后面一行跟默认值序列,必须是0~255或0x0~0xff\n\
\n\
[Information]\n\
Name=自定义标签页名字\n\
\n\
[VfyPwd]\n\
CmdCode=0x13\n\
Params=0x00,0,0x0,0\n\
\n\
[SetPwd]\n\
CmdCode=0x12\n\
Params=0,0,0,0\n\
\n\
[GetRandomCode]\n\
CmdCode=0x14\n\
Params=\n\
\n\
[Search]\n\
CmdCode=0x04\n\
Params=1,0,0,0,0x87\n\
\n\
[UpImage]\n\
CmdCode=0x0a\n\
Params=\n\
\n\
[Empty]\n\
CmdCode=0x0d\n\
Params=\n\
\n\
[WriteReg]\n\
CmdCode=0x0e\n\
Params=4,6\n\
\n\
[ReadSysPara]\n\
CmdCode=0x0f\n\
Params=\n\
\n\
[SetChipAddr]\n\
CmdCode=0x15\n\
Params=0,0,0,0\n\
\n\
[ReadINFPage]\n\
CmdCode=0x16\n\
Params=\n\
";
    return OperateFile(path,"w",[&](FILE* fp){fprintf(fp,"%s",str1);});
}

bool MyFile::SaveDefaultPlugin2(MyString path){
    char* str2="# 这个文件必须是ANSI格式的\n\
# 井号用来注释\n\
# 但是只能注释一行,不能行内注释,会解释出错\n\
# Information节必须存在,而且必须有Name键值\n\
# 每个命令节必须有CmdCode,而且值是一个十六进制表示的数字\n\
# 后面一行跟默认值序列,必须是0~255或0x0~0xff\n\
\n\
[Information]\n\
Name=自定义标签页名字2\n\
\n\
[ReadNotepad]\n\
CmdCode=0x19\n\
Params=15\n\
\n\
[ValidTempleteNum]\n\
CmdCode=0x1d\n\
Params=\n\
\n\
[ReadIndexTable]\n\
CmdCode=0x1f\n\
Params=3\n\
\n\
[ControlLED]\n\
CmdCode=0x40\n\
Params=15\n\
\n\
[UpChar]\n\
CmdCode=0x08\n\
Params=1\n\
";
    return OperateFile(path,"w",[&](FILE* fp){fprintf(fp,"%s",str2);});
}

bool MyFile::SaveAsDialog(MyString defaultName,MyString extName,CWnd* parent,MyString & path){
    MyString Filter=extName+"文件(*."+extName+")|*."+extName+"|所有文件(*.*)|*.*||";
    CFileDialog fileDialog(FALSE,extName,defaultName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,Filter,parent);
    if(IDOK==fileDialog.DoModal()){
        path=fileDialog.GetPathName();
        return true;
    }
    return false;
}

bool MyFile::OpenFileDialog(MyString extName,CWnd * parent,MyString & path){
    MyString Filter=extName+"文件(*."+extName+")|*."+extName+"|所有文件(*.*)|*.*||";
    CFileDialog fileDialog(TRUE,extName,L"",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST,Filter,parent);
    if(IDOK==fileDialog.DoModal()){
        path=fileDialog.GetPathName();
        return true;
    }
    return false;
}

bool MyFile::ReadImage(MyString imagePath,DataPacket& data){
    ReturnFileFunction_t f=[&](FILE* fp)->bool{
        int w,h;
        //跳过位图文件头结构BITMAPFILEHEADER
        fseek(fp,sizeof(BITMAPFILEHEADER),SEEK_SET);
        //定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
        BITMAPINFOHEADER head;
        fread(&head,sizeof(BITMAPINFOHEADER),1,fp); //获取图像宽、高、每像素所占位数等信息
        w=head.biWidth;
        h=head.biHeight;
        if(head.biBitCount!=8){
            return false;
        }
        fseek(fp,256*sizeof(RGBQUAD),SEEK_CUR);
        //申请位图数据所需要的空间，读位图数据进内存
        uint8_t* p=new uint8_t[w*h];
        fread(p,1,w*h,fp);
        uint8_t* divBy2=new uint8_t[w*h/2];
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(j%2==0){
                    divBy2[(i*w+j)/2]=p[i*w+j];
                } else{
                    divBy2[(i*w+j)/2]|=(p[i*w+j]&0xF0)>>4;
                }
            }
        }
        data=DataPacket(divBy2,w*h/2);
        return true;
    };
    return OperateFile(imagePath,"rb",f);
}

bool MyFile::SaveCharFile(MyString charPath,DataPacket data){
    FileFunction_t f=[&](FILE* fp){
        fwrite(data.getPointer(),1,data.readSize(),fp);
    };
    return OperateFile(charPath,"wb",f);
}

bool MyFile::LoadCharFile(MyString charPath,DataPacket& dataPacket){
    FileFunction_t f=[&](FILE* fp){
        uint8_t data[1536];
        fread(data,1,1536,fp);
        dataPacket=DataPacket(data,1536);
    };
    return OperateFile(charPath,"rb",f);
}
