#pragma once
#include<string>

// 管理日益复杂的字符串转化

// 字符串模块
class MyString{
public:
    // 析构函数
    ~MyString();
    // 构造函数群
    MyString();
    MyString(char* pc);
    MyString(char* pc,int len);
    MyString(wchar_t* pw);
    MyString(CString pCS);
    MyString(const MyString& other);
    MyString(MyString&& other);
    MyString(const char* pcc);
    MyString(std::string str);

    // 类似sprintf
    static MyString Format(const char* format,...);

    // 类似sscanf
    void Parse(const char* format,...);

    // 转化操作符
    operator wchar_t*();
    operator const char*();
    operator const wchar_t*();
    operator CString();

    // 重载关系操作符
    bool operator==(const MyString other)const;
    bool operator==(MyString other);
    bool operator==(const char* other);
    bool operator!=(MyString other);
    bool operator!=(const char* other);

    // 重载连接操作符
    MyString operator+(MyString other);

    // 重载赋值操作符
    MyString& operator=(MyString other);

    // 重载加等操作符
    MyString& operator+=(MyString other);

    // 返回长度
    int length();

    // 查找函数
    int find(MyString beFound,int startPosition);

    // int转MyString
    static MyString IntToMyString(int i);

    // MyString转int
    static int ParseInt(MyString s);

    // 构造时间字符串
    static MyString Time(const char* format="%Y_%m_%d_%H_%M_%S");
private:
    // 实际存储内容的std::string
    std::string str;
    // 负责返回宽字符
    wchar_t* wstr=0;
    // 根据str更新wstr
    void updateWstr();
};