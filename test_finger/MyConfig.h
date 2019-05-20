#pragma once
#include"stdafx.h"
class MyConfig{
private:
    const char* FILENAME="config";
    static char buffer[1<<16];
    static char key[1<<6];
    static char val[1<<6];
    std::map<std::string,std::string>m;
    void defaultConfig();
public:
    MyConfig();
    ~MyConfig();
    std::string& operator[](std::string s);

};

extern MyConfig conf;