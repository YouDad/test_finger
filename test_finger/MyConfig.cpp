#include "stdafx.h"

char MyConfig::buffer[1<<16];
char MyConfig::key[1<<6];
char MyConfig::val[1<<6];
MyConfig conf;

void MyConfig::defaultConfig(){
    m["AdvDbg"]="false";
    m["AdvDbg_ImgId"]="0";
    m["AutoCheck"]="true";
    return;
}

MyConfig::MyConfig(){
    FILE* fp=fopen(FILENAME,"r");
    if(fp==NULL){
        defaultConfig();
        return;
    }
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