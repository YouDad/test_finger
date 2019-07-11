#include "stdafx.h"

// 全局唯一配置
MyConfig conf;

void setDefault(std::map<std::string,std::string>& m,const char* key,const char* val,bool give,bool cond=true){
    if(give||!m.count(key)){
        if(cond){
            m[key]=val;
        }
    }
}

// 默认配置
void MyConfig::defaultConfig(){
    if(!m.count("SaveConf")){
        m["SaveConf"]=Stringify(false);
    }
    // 若SaveConf为false,则都用默认设置
    bool give=m["SaveConf"]==Stringify(false);

    setDefault(m,"CustomCnt","0",give);
    setDefault(m,"AdvDbg",Stringify(false),give);
    setDefault(m,"AutoCheck",Stringify(true),give);
    setDefault(m,"AutoLog",Stringify(true),give);
    setDefault(m,"RemBaud",Stringify(true),give);
    setDefault(m,"RemProtocol",Stringify(true),give);
    setDefault(m,"Baud","2",give,m["RemBaud"]==Stringify(true));
    setDefault(m,"ProtocolType","0",give,m["RemProtocol"]==Stringify(true));
    setDefault(m,"ImgSize","160",give);
    return;
}

// 初始化函数是从文件中读取配置
void MyConfig::InitConfig(){
    MyFile::ReadConfig(
        [&](FILE* fp){
            char buffer[1<<16],key[1<<6],val[1<<6];
            char* p=buffer;
            // 一下子全部取负读入
            while((*p++=-fgetc(fp))!=-EOF);
            // 补零回退,补零是为了结束字符串
            *--p=0;
            int n,c=0;
            while(buffer[c]){
                sscanf_s(buffer+c,"%[^`]`%[^`]`%n",key,sizeof key,val,sizeof val,&n);
                c+=n;
                m[key]=val;
            }
        }
    );
    defaultConfig();
}

// 析构函数是把配置写到文件里
MyConfig::~MyConfig(){
    if(m["SaveConf"]==Stringify(true)){
        MyFile::WriteConfig(
            [&](FILE* fp){
                for(auto it=m.begin();it!=m.end();it++){
                    if(it->second==""){
                        continue;
                    }
                    for(auto jt=it->first.c_str();*jt;jt++){
                        fputc(-*jt,fp);
                    }
                    fputc(-'`',fp);
                    for(auto jt=it->second.c_str();*jt;jt++){
                        fputc(-*jt,fp);
                    }
                    fputc(-'`',fp);
                }
            }
        );
    }
}

// 像map<string,string>一样,键类型是字符串,值类型也是字符串
std::string& MyConfig::operator[](std::string s){
    return m[s];
}

std::string MyConfig::Bool(bool b){
    return b?Stringify(true):Stringify(false);
}
