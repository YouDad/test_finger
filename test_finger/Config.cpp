#include "stdafx.h"
#include "Config.h"

char $::Config::buffer[1<<16];
char $::Config::key[1<<6];
char $::Config::val[1<<6];
$::Config $::conf;

void $::Config::defaultConfig(){
    m["AdvDbg"]="false";
    m["AdvDbg_ImgId"]="0";
    return;
}

$::Config::Config(){
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

$::Config::~Config(){
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

std::string& $::Config::operator[](std::string s){
    return m[s];
}