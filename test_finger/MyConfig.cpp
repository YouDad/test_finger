#include "stdafx.h"

// 静态成员声明
char MyConfig::buffer[1<<16];
char MyConfig::key[1<<6];
char MyConfig::val[1<<6];
// 全局唯一配置
MyConfig conf;

// 默认配置
void MyConfig::defaultConfig(){
    if(!m.count("AdvDbg")){
        m["AdvDbg"]="false";
    }
    if(!m.count("AdvDbg_ImgId")){
        m["AdvDbg_ImgId"]="0";
    }
    if(!m.count("AutoCheck")){
        m["AutoCheck"]="true";
    }
    if(!m.count("Baud")){
        m["Baud"]="2";
    }
    if(!m.count("ProtocolType")){
        m["ProtocolType"]="2";
    }
    return;
}

// 构造函数是从文件中读取配置
MyConfig::MyConfig(){
    FILE* fp=fopen(FILENAME,"r");
    if(fp){
        char*p=buffer;
        while((*p++=-fgetc(fp))!=-EOF);
        *--p=0;
        fclose(fp);
        int n,c=0;
        while(buffer[c]){
            sscanf(buffer+c,"%[^`]`%[^`]`%n",key,val,&n);
            c+=n;
            m[key]=val;
        }
    }
    defaultConfig();
}

// 析构函数是把配置写到文件里
MyConfig::~MyConfig(){
    FILE* fp=fopen(FILENAME,"w");
    if(!fp){
        return;
    }
    for(auto it=m.begin();it!=m.end();it++){
        for(auto jt=it->first.c_str();*jt;jt++){
            fputc(-*jt,fp);
        }
        fputc(-'`',fp);
        for(auto jt=it->second.c_str();*jt;jt++){
            fputc(-*jt,fp);
        }
        fputc(-'`',fp);
    }
    fclose(fp);
}

// 像map<string,string>一样,键类型是字符串,值类型也是字符串
std::string& MyConfig::operator[](std::string s){
    return m[s];
}