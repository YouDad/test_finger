#include "stdafx.h"
// ������������Ҫ�Ŀ�
#include <afxinet.h>

// ixv������������ַ
MyString HostIP="39.107.64.93";

// ����ص��������Ͷ���
#define NetFunction_t _Function_t(void,uint8_t* data,int size)
#define NetFunction _Function(void,uint8_t*data,int size)
// �����ֵ�����
typedef std::map<std::string,std::string> DataMap;

// Post����
void NetPost(MyString host,MyString ctrl,DataMap data,NetFunction_t func){
    // ��������
    CInternetSession session;
    DWORD dwStatusCode;
    INTERNET_PORT nPort=80;
    CHttpConnection* pserver=session.GetHttpConnection(host,nPort);
    CHttpFile* pfile=pserver->OpenRequest(CHttpConnection::HTTP_VERB_POST,ctrl);
    CString strHeaders=_T("Content-Type: application/x-www-form-urlencoded");

    // ƴ��a=b&c=d����ʽ
    std::string dataStr;
    for(auto it=data.begin();it!=data.end();it++){
        dataStr+=it->first+"="+it->second+"&";
    }

    // ɾ�����һ��&
    if(dataStr.length()){
        dataStr.pop_back();
    }

    try{
        // ����Post����
        pfile->SendRequest(strHeaders,(LPVOID)dataStr.c_str(),dataStr.length());
        pfile->QueryInfoStatusCode(dwStatusCode);
        if(dwStatusCode==HTTP_STATUS_OK){
            uint8_t data[1024];
            int size=0;
            while((size=pfile->Read(data,1024))>0){
                // ʹ�ûص�������������
                func(data,size);
            }
        }
    } catch(...){}
    // �ر�����
    pfile->Close();
    delete pfile;
    pserver->Close();
    delete pserver;
}

// ������߰汾
int NetGetVersion(){
    return 0;
    int BigVersion=0,SmlVersion=0;
    NetPost(HostIP,"test_finger_version",DataMap(),[&](uint8_t* data,int size){
        MyString((char*)data,size).Parse("%d.%d",&BigVersion,&SmlVersion);
        });
    if(SmlVersion<10){
        SmlVersion*=10;
    }
    return BigVersion*100+SmlVersion;
}

// ���ذ汾�ĺ���,����Ϊ��Ӧ�汾��ÿ�λ�����ݵĻص�����
void NetDownload(int NetVersion,_Function_t(void,uint8_t* data,int size,int total) func){
    int BigVersion,SmlVersion;
    BigVersion=NetVersion/100;
    SmlVersion=NetVersion%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    int fileSize;
    DataMap data;
    data["Version"]=(const char*)MyString::Format("%d.%d",BigVersion,SmlVersion);
    NetPost(HostIP,"test_finger_size",data,[&](uint8_t* data,int size)->void{
        MyString((char*)data,size).Parse("%d",&fileSize);
        });

    NetPost(HostIP,"test_finger_get",data,[&](uint8_t* data,int size)->void{
        func(data,size,fileSize);
        });
}

// ��Unicodeת����ANSI
std::string UnicodeToANSI(const std::wstring & wstr){
    std::string ret;
    std::mbstate_t state={};
    const wchar_t *src=wstr.data();
    size_t len=std::wcsrtombs(nullptr,&src,0,&state);
    if(static_cast<size_t>(-1)!=len){
        std::unique_ptr< char[] > buff(new char[len+1]);
        len=std::wcsrtombs(buff.get(),&src,len,&state);
        if(static_cast<size_t>(-1)!=len){
            ret.assign(buff.get(),len);
        }
    }
    return ret;
}

// ��ð汾��ϸ˵��
MyString NetVersionInfo(int NetVersion){
    int BigVersion,SmlVersion;
    BigVersion=NetVersion/100;
    SmlVersion=NetVersion%100;
    if(SmlVersion%10==0){
        SmlVersion/=10;
    }
    DataMap data;
    data["Version"]=(const char*)MyString::Format("%d.%d",BigVersion,SmlVersion);

    MyString ret;
    NetPost(HostIP,"test_finger_info",data,[&](uint8_t* data,int size){
        ret+=MyString((char*)data,size);
        });
    // �����ַ�����
    const char* p=ret;
    // ����unicodeʱ�õ�
    char tmp_str[7]={};
    WCHAR* tmp=new WCHAR[ret.length()+1];
    int tmp_i;
    for(tmp_i=0;*p;tmp_i++){
        if(p[0]=='\\'&&p[1]=='u'){
            memcpy(tmp_str,p,6);
            sscanf(tmp_str,"\\u%x",(unsigned int*)(tmp+tmp_i));
            p+=6;
        } else if(p[0]=='\\'&&p[1]=='n'){
            tmp[tmp_i]='\n';
            p+=2;
        } else{
            tmp[tmp_i]=p[0];
            p++;
        }
    }
    // --��+1Ϊ��ȥ��һ�����ڵ�˫����
    tmp[--tmp_i]=0;
    return MyString(tmp+1);
}

// �����Ƿ���������
bool isConnectedNet(){
    DWORD flags;
    return InternetGetConnectedState(&flags,0);
}
