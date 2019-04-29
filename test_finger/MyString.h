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
private:
    std::string str;
    wchar_t* wstr=0;
    void updateWstr();
};