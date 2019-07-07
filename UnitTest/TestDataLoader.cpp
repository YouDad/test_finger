#include "stdafx.h"

TestDataLoader::TestDataLoader(const char * func){
    FILE* fp=fopen(MyString("../TestData/")+func+".dat","r");
    char k[1024],v[1<<18];
    while(~fscanf_s(fp,"[%[^]]]\n%[^[]",k,1024,v,1<<18)){
        sections[k]=v;
    }
    fclose(fp);
}

std::vector<uint8_t> TestDataLoader::ReadByteSection(const char * section){
    std::string& _s=sections[section];
    const char* s=_s.c_str();
    std::vector<uint8_t> ret;
    int t;
    for(int i=0;i<_s.size()/5;i++){
        sscanf(s+i*5,"%x",&t);
        ret.push_back(t);
    }
    return ret;
}

std::vector<MyString> TestDataLoader::ReadStrSection(const char * section){
    std::string& _s=sections[section];
    const char* s=_s.c_str();
    std::vector<int>v;
    v.push_back(0);
    for(int j=0;s[j];j++){
        if(s[j]=='\n'){
            v.push_back(j+1);
            _s[j]=0;
        }
    }
    s=_s.c_str();
    std::vector<MyString> ret;
    for(int i=0;i<v.size();i++){
        ret.push_back(s+v[i]);
    }
    if(ret.back()==""){
        ret.pop_back();
    }
    return ret;
}
