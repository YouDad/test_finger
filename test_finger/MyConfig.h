#pragma once
#include <map>

// 配置类
class MyConfig{
private:
    const char* FILENAME="config";
    static char buffer[1<<16];
    static char key[1<<6];
    static char val[1<<6];
    // 保存配置的地方
    std::map<std::string,std::string>m;
    // 默认配置
    void defaultConfig();
public:
    MyConfig();
    ~MyConfig();
    // 重载[]操作符
    std::string& operator[](std::string s);

};

// 全局唯一配置
extern MyConfig conf;