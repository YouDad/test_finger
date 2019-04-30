#pragma once
#include"stdafx.h"
#include<string>

//管理日益复杂的字符串转化

class MyString{
public:
    ~MyString();
    MyString();
    MyString(char* pc);
    MyString(char* pc,int len);
    MyString(wchar_t* pw);
    MyString(CString pCS);
    MyString(MyString& other);
    MyString(MyString&& other);
    MyString(const char* pcc);
    MyString(std::string str);
    static MyString Format(const char* format,...);
    operator wchar_t*();
    operator const char*();
    operator const wchar_t*();
    operator CString();
    bool operator==(MyString other);
    bool operator==(const char* other);
    MyString operator+(MyString other);
    MyString& operator=(MyString other);
    int length();
    int find(MyString beFound,int startPosition);
    static MyString IntToMyString(int i);
    static int ParseInt(MyString s);
    static MyString Time(const char* format="%Y_%m_%d_%H_%M_%S");
private:
    std::string str;
    wchar_t* wstr=0;
    void updateWstr();
};