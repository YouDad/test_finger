#pragma once
#include"stdafx.h"
#include<string>

//管理日益复杂的字符串转化

class MyString{
public:
    MyString(const char* pcc){
        str=pcc;
    }
    MyString(char* pc){
        str=pc;
    }
    MyString(wchar_t* pw){

    }
private:
    std::string str;
};