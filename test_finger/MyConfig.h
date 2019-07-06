#pragma once
#include <map>

// 配置类
class MyConfig{
private:
    // 保存配置的地方
    std::map<std::string,std::string>m;
    // 默认配置
    void defaultConfig();
public:
    void InitConfig();
    ~MyConfig();
    // 重载[]操作符
    std::string& operator[](std::string s);
    // 提供字符串表示
    static std::string Bool(bool b);
};

// 全局唯一配置
extern MyConfig conf;