#include "stdafx.h"

IMPLEMENT_DYNAMIC(SettingDialog,CDialogEx)

SettingDialog::SettingDialog(CWnd* pParent)
    : CDialogEx(IDD_SETTING_DIALOG,pParent){}

SettingDialog::~SettingDialog(){}

CButton* btnCheckUpdate;
CButton* btnDownload;
CStatic* UpdateInfo;
CButton* chkAutoCheck;
CStatic* DownloadDetail;
CProgressCtrl* DownloadProgress;

BOOL SettingDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    btnCheckUpdate=(CButton*)GetDlgItem(IDC_BTNCheckUpdate);
    btnDownload=(CButton*)GetDlgItem(IDC_BTNDownload);
    UpdateInfo=(CStatic*)GetDlgItem(IDC_UpdateInfo);
    chkAutoCheck=(CButton*)GetDlgItem(IDC_CHKAutoCheck);
    DownloadDetail=(CStatic*)GetDlgItem(IDC_DownloadDetail);
    DownloadProgress=(CProgressCtrl*)GetDlgItem(IDC_DownloadProgress);

    DownloadProgress->SetPos(0);
    DownloadProgress->SetRange(0,100);

    if(conf["AutoCheck"]=="true"){
        chkAutoCheck->SetCheck(1);
    } else{
        chkAutoCheck->SetCheck(0);
    }
    return 0;
}

void SettingDialog::OnOK(){}

BEGIN_MESSAGE_MAP(SettingDialog,CDialogEx)
    ON_BN_CLICKED(IDC_BTNCheckUpdate,&SettingDialog::OnBnClickedBtncheckupdate)
    ON_BN_CLICKED(IDC_BTNDownload,&SettingDialog::OnBnClickedBtndownload)
    ON_BN_CLICKED(IDC_CHKAutoCheck,&SettingDialog::OnBnClickedChkautocheck)
END_MESSAGE_MAP()


void SettingDialog::OnBnClickedBtncheckupdate(){
    int NetVersion=NetGetVersion();
    if(NetVersion==Version){
        setText(UpdateInfo,"现在已是最新版本");
    } else if(NetVersion<Version){
        setText(UpdateInfo,"现在是实验版本");
    } else{
        setText(UpdateInfo,"发现新版本");
    }
}


void SettingDialog::OnBnClickedBtndownload(){
    NetDownload(DownloadProgress,DownloadDetail);
}


void SettingDialog::OnBnClickedChkautocheck(){
    if(chkAutoCheck->GetCheck()){
        conf["AutoCheck"]="true";
    } else{
        conf["AutoCheck"]="false";
    }
}
