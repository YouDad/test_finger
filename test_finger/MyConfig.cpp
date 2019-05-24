#include "stdafx.h"

char MyConfig::buffer[1<<16];
char MyConfig::key[1<<6];
char MyConfig::val[1<<6];
MyConfig conf;

void MyConfig::defaultConfig(){
    if(m.count("AdvDbg")){
        m["AdvDbg"]="false";
    }
    if(m.count("AdvDbg_ImgId")){
        m["AdvDbg_ImgId"]="0";
    }
    if(m.count("AutoCheck")){
        m["AutoCheck"]="true";
    }
    if(m.count("Baud")){
        m["Baud"]="2";
    }
    if(m.count("ProtocolType")){
        m["ProtocolType"]="2";
    }
    return;
}

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

MyConfig::~MyConfig(){
    FILE* fp=fopen(FILENAME,"w");
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

std::string& MyConfig::operator[](std::string s){
    return m[s];
}