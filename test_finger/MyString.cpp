#include "stdafx.h"

// 析构函数
MyString::~MyString(){
    try{
        if(wstr){
            delete[] wstr;
        }
    } catch(...){

    }
}

// 构造函数群
MyString::MyString(){
    str="";
    updateWstr();
}

MyString::MyString(const char* pcc){
    str=pcc;
    updateWstr();
}

MyString::MyString(std::string str){
    this->str=str;
    updateWstr();
}

MyString::MyString(char* pc){
    str=pc;
    updateWstr();
}

MyString::MyString(char * pc,int len){
    uint8_t b=pc[len];
    pc[len]=0;
    str=pc;
    updateWstr();
    pc[len]=b;
}

MyString::MyString(wchar_t* pw){
    int size=WideCharToMultiByte(CP_ACP,0,pw,-1,NULL,0,NULL,NULL);
    char* pc=new char[size];
    WideCharToMultiByte(CP_ACP,0,pw,-1,pc,size,NULL,NULL);
    str=pc;
    delete[] pc;
    updateWstr();
}

MyString::MyString(CString pCS){
    *this=MyString(pCS.GetBuffer(pCS.GetLength()));
}

MyString::MyString(const MyString & other){
    str=other.str;
    updateWstr();
}

// 构造函数群
MyString::MyString(MyString && other){
    str=other.str;
    updateWstr();
}

// 类似sprintf
MyString MyString::Format(const char * format,...){
    static char tmp[1<<16];
    va_list ap;
    va_start(ap,format);
    vsprintf(tmp,format,ap);
    va_end(ap);
    return MyString(tmp);
}

// 类似sscanf
void MyString::Parse(const char * format,...){
    va_list ap;
    va_start(ap,format);
    vsscanf(str.c_str(),format,ap);
    va_end(ap);
}

// 转化操作符
MyString::operator wchar_t*(){
    return wstr;
}

MyString::operator const char*()const{
    return str.c_str();
}

MyString::operator const wchar_t*(){
    return wstr;
}

MyString::operator CString(){
    return CString(str.c_str());
}

bool MyString::operator==(const MyString other) const{
    return str==other.str;
}

// 重载关系操作符
bool MyString::operator==(MyString other){
    return str==other.str;
}

bool MyString::operator==(const char * other){
    return *this==MyString(other);
}

bool MyString::operator!=(MyString other){
    return !operator==(other);
}

bool MyString::operator!=(const char * other){
    return !operator==(other);
}

// 重载连接操作符
MyString MyString::operator+(MyString other){
    MyString ret(*this);
    ret.str+=other.str;
    ret.updateWstr();
    return ret;
}

// 重载赋值操作符
MyString & MyString::operator=(MyString other){
    str=other.str;
    updateWstr();
    return *this;
}

// 重载加等操作符
MyString & MyString::operator+=(MyString other){
    return *this=*this+other;
}

const char MyString::operator[](int index){
    return str[index];
}

// 返回长度
int MyString::length(){
    return str.length();
}

// 查找函数
int MyString::find(MyString beFound,int startPosition){
    return str.find(beFound.str,startPosition);
}

// 替换函数
void MyString::replace(char oldChar,char newChar){
    for(int i=0;i<str.size();i++){
        if(str[i]==oldChar){
            str[i]=newChar;
        }
    }
    updateWstr();
}

void MyString::split(char ch,std::vector<MyString>& out){
    out.clear();
    for(int i=0,j;i<str.length();i=j+1){
        j=str.find(ch,i);
        if(j==-1){
            out.push_back(MyString(str.c_str()+i));
            return;
        }
        out.push_back(MyString((char*)str.c_str()+i,j-i));
    }
}

const char * MyString::c_str(){
    return str.c_str();
}

// int转MyString
MyString MyString::IntToMyString(int i){
    CString x;
    x.Format(_T("%d"),i);
    return x;
}

// MyString转int
int MyString::ParseInt(MyString s){
    int ret=0;
    sscanf(s,"%d",&ret);
    return ret;
}

int MyString::AutoParseInt(MyString s){
    int ret=0;
    if( (s[0]=='0'&&(s[1]=='x'||s[1]=='X')) ||
        (('a'<=(s[0]|32)&&(s[0]|32)<='f')) ){
        sscanf(s,"%x",&ret);
    } else{
        sscanf(s,"%d",&ret);
    }
    return ret;
}

// 构造时间字符串
MyString MyString::Time(const char * format){
    return CTime::GetCurrentTime().Format(format);
}

// 根据str更新wstr
void MyString::updateWstr(){
    if(wstr){
        delete[] wstr;
    }
    int size=MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
    wstr=new wchar_t[size];
    MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,wstr,size);
}