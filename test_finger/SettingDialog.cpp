#include "stdafx.h"

IMPLEMENT_DYNAMIC(SettingDialog,CDialogEx)

// 构造函数
SettingDialog::SettingDialog(CWnd* pParent) : CDialogEx(IDD_SETTING_DIALOG,pParent){}

// 析构函数
SettingDialog::~SettingDialog(){}

// 控件指针群
CButton* btnCheckUpdate;
CButton* btnDownload;
CStatic* UpdateInfo;
CButton* chkAutoCheck;
CStatic* DownloadDetail;
CStatic* VersionInfo;
CProgressCtrl* DownloadProgress;

// 初始化事件
BOOL SettingDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    btnCheckUpdate=(CButton*)GetDlgItem(IDC_BTNCheckUpdate);
    btnDownload=(CButton*)GetDlgItem(IDC_BTNDownload);
    UpdateInfo=(CStatic*)GetDlgItem(IDC_UpdateInfo);
    chkAutoCheck=(CButton*)GetDlgItem(IDC_CHKAutoCheck);
    DownloadDetail=(CStatic*)GetDlgItem(IDC_DownloadDetail);
    VersionInfo=(CStatic*)GetDlgItem(IDC_VersionInfo);
    DownloadProgress=(CProgressCtrl*)GetDlgItem(IDC_DownloadProgress);

    DownloadProgress->SetPos(0);
    DownloadProgress->SetRange(0,100);

    if(conf["AutoCheck"]=="true"){
        chkAutoCheck->SetCheck(TRUE);
    } else{
        chkAutoCheck->SetCheck(FALSE);
    }
    return 0;
}

// 阻止Enter关闭窗口
void SettingDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(SettingDialog,CDialogEx)
    ON_BN_CLICKED(IDC_BTNCheckUpdate,&SettingDialog::OnBnClickedBtnCheckUpdate)
    ON_BN_CLICKED(IDC_BTNDownload,&SettingDialog::OnBnClickedBtnDownload)
    ON_BN_CLICKED(IDC_CHKAutoCheck,&SettingDialog::OnBnClickedChkAutoCheck)
END_MESSAGE_MAP()

// 确认更新点击事件
void SettingDialog::OnBnClickedBtnCheckUpdate(){
    int NetVersion=NetGetVersion();
    if(NetVersion==Version){
        setText(UpdateInfo,"现在已是最新版本");
    } else if(NetVersion<Version){
        setText(UpdateInfo,"现在是实验版本");
    } else{
        setText(UpdateInfo,"发现新版本");
        setText(VersionInfo,NetVersionInfo(NetVersion));
    }
}

// 把整数个字节转化为人比较好读的字符串
MyString description(int size){
    if(size>1024LL*1024*1024*8/7){
        return MyString::Format("%.2lfGB",1.0*size/1024/1024/1024);
    } else if(size>1024LL*1024*8/7){
        return MyString::Format("%.2lfMB",1.0*size/1024/1024);
    } else if(size>1024LL*8/7){
        return MyString::Format("%.2lfKB",1.0*size/1024);
    } else{
        return MyString::Format("%dB",size);
    }
}

// 下载按钮的点击事件
void SettingDialog::OnBnClickedBtnDownload(){
    // 先给缺陷找理由
    if(2==MessageBoxA(0,"下载文件时不能操作该程序,即便如此也要下载么?",
                      "询问",MB_ICONEXCLAMATION|MB_OKCANCEL|MB_SYSTEMMODAL)){
        return;
    }
    // 确定版本
    int NetVersion=NetGetVersion();
    if(Version>NetVersion){
        if(2==MessageBoxA(0,"当前版本比远程版本高,即便如此也要下载么?",
                          "浪费流量警告",MB_ICONEXCLAMATION|MB_OKCANCEL|MB_SYSTEMMODAL)){
            return;
        }
    } else if(Version==NetVersion){
        MessageBoxA(0,"当前版本和远程版本一样","不能下载",MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
        return;
    }
    // 准备下载
    int BigVersion,SmlVersion;
    BigVersion=NetVersion/100;
    SmlVersion=NetVersion%100;
    FILE* fp=fopen(MyString::Format("test_fingerV%d.%d.release.exe",BigVersion,SmlVersion),"wb");
    int now=0;
    // 下载,带上回调函数
    NetDownload(NetVersion,[&](uint8_t* data,int size,int total)->void{
        fwrite(data,1,size,fp);
        now+=size;
        DownloadProgress->SetPos(100*now/total);
        MyString nowStr=description(now);
        MyString sumStr=description(total);
        setText(DownloadDetail,nowStr+"/"+sumStr);
    });
    fclose(fp);
    MessageBoxA(0,"下载完毕!","OK",MB_OK);
}

// 自动更新Check的点击事件
void SettingDialog::OnBnClickedChkAutoCheck(){
    if(chkAutoCheck->GetCheck()){
        conf["AutoCheck"]="true";
    } else{
        conf["AutoCheck"]="false";
    }
}
