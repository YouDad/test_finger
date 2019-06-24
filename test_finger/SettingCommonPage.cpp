#include "stdafx.h"
#include "SettingCommonPage.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(SettingCommonPage,CDialogEx)

// 构造函数
SettingCommonPage::SettingCommonPage(CWnd* pParent): CDialogEx(IDD_SettingCommonPage,pParent){}

// 析构函数
SettingCommonPage::~SettingCommonPage(){}

// 控件指针群
CButton* chkSaveConf;
CButton* chkAutoCheck;
CButton* chkAutoLog;
CButton* chkRemBaud;
CButton* chkRemProtocol;
CButton* btnQuitAdvDbg;
CButton* btnApply;
CStatic* txtImgSize;
CSliderCtrl* sliderImgSize;

// 初始化事件
BOOL SettingCommonPage::OnInitDialog(){
    CDialogEx::OnInitDialog();
    chkSaveConf=(CButton*)GetDlgItem(IDC_CHKSaveConf);
    chkAutoCheck=(CButton*)GetDlgItem(IDC_CHKAutoCheck);
    chkAutoLog=(CButton*)GetDlgItem(IDC_CHKAutoLog);
    chkRemBaud=(CButton*)GetDlgItem(IDC_CHKRemBaud);
    chkRemProtocol=(CButton*)GetDlgItem(IDC_CHKRemProtocol);
    btnQuitAdvDbg=(CButton*)GetDlgItem(IDC_BTNQuitAdvDbg);
    btnApply=(CButton*)GetDlgItem(IDC_BTNApply);
    txtImgSize=(CStatic*)GetDlgItem(IDC_STATICimgSize);
    sliderImgSize=(CSliderCtrl*)GetDlgItem(IDC_SLIDERimgSize);

    sliderImgSize->SetRange(4,40);
    sliderImgSize->SetLineSize(1);
    sliderImgSize->SetPageSize(4);
    sliderImgSize->SetTicFreq(4);

    OnBnClickedBtnCancel();
    OnBnClickedChkSaveConf();
    btnQuitAdvDbg->ShowWindow(conf["AdvDbg"]==Stringify(true)?TRUE:FALSE);
    btnApply->EnableWindow(FALSE);
    return 0;
}

// 阻止Enter关闭窗口
void SettingCommonPage::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(SettingCommonPage,CDialogEx)
    ON_BN_CLICKED(IDC_BTNApply,&SettingCommonPage::OnBnClickedBtnApply)
    ON_BN_CLICKED(IDC_BTNCancel,&SettingCommonPage::OnBnClickedBtnCancel)
    ON_BN_CLICKED(IDC_BTNQuitAdvDbg,&SettingCommonPage::OnBnClickedBtnQuitAdvDbg)
    ON_BN_CLICKED(IDC_CHKSaveConf,&SettingCommonPage::OnBnClickedChkSaveConf)
    ON_BN_CLICKED(IDC_CHKAutoCheck,&SettingCommonPage::OnBnClickedChkautocheck)
    ON_BN_CLICKED(IDC_CHKAutoLog,&SettingCommonPage::OnBnClickedChkautolog)
    ON_BN_CLICKED(IDC_CHKRemBaud,&SettingCommonPage::OnBnClickedChkrembaud)
    ON_BN_CLICKED(IDC_CHKRemProtocol,&SettingCommonPage::OnBnClickedChkremprotocol)
    ON_NOTIFY(NM_CUSTOMDRAW,IDC_SLIDERimgSize,&SettingCommonPage::OnNMCustomdrawSliderImgSize)
END_MESSAGE_MAP()


void SettingCommonPage::OnBnClickedBtnApply(){
    conf["SaveConf"]=MyConfig::Bool(isChecked(chkSaveConf));
    if(isChecked(chkSaveConf)){
        conf["AutoCheck"]=MyConfig::Bool(isChecked(chkAutoCheck));
        conf["AutoLog"]=MyConfig::Bool(isChecked(chkAutoLog));
        conf["RemBaud"]=MyConfig::Bool(isChecked(chkRemBaud));
        conf["RemProtocol"]=MyConfig::Bool(isChecked(chkRemProtocol));
        conf["ImgSize"]=MyString::IntToMyString(sliderImgSize->GetPos()*10);
    } else{
        MyFile::WriteConfig([](FILE* fp){});
    }
    btnApply->EnableWindow(FALSE);
}


void SettingCommonPage::OnBnClickedBtnCancel(){
    setCheck(chkSaveConf,conf["SaveConf"]==Stringify(true));
    setCheck(chkAutoCheck,conf["AutoCheck"]==Stringify(true));
    setCheck(chkAutoLog,conf["AutoLog"]==Stringify(true));
    setCheck(chkRemBaud,conf["RemBaud"]==Stringify(true));
    setCheck(chkRemProtocol,conf["RemProtocol"]==Stringify(true));

    sliderImgSize->SetPos(MyString::ParseInt(conf["ImgSize"])/10);

    OnBnClickedChkSaveConf();
}


void SettingCommonPage::OnBnClickedBtnQuitAdvDbg(){
    conf["AdvDbg"]=Stringify(false);
    btnAdvDbg->ShowWindow(SW_HIDE);
    btnQuitAdvDbg->ShowWindow(SW_HIDE);
}


void SettingCommonPage::OnBnClickedChkSaveConf(){
    chkAutoCheck->EnableWindow(isChecked(chkSaveConf));
    chkAutoLog->EnableWindow(isChecked(chkSaveConf));
    chkRemBaud->EnableWindow(isChecked(chkSaveConf));
    chkRemProtocol->EnableWindow(isChecked(chkSaveConf));
    sliderImgSize->EnableWindow(isChecked(chkSaveConf));
    btnApply->EnableWindow(TRUE);
}


void SettingCommonPage::OnBnClickedChkautocheck(){
    btnApply->EnableWindow(TRUE);
}


void SettingCommonPage::OnBnClickedChkautolog(){
    btnApply->EnableWindow(TRUE);
}


void SettingCommonPage::OnBnClickedChkrembaud(){
    btnApply->EnableWindow(TRUE);
}


void SettingCommonPage::OnBnClickedChkremprotocol(){
    btnApply->EnableWindow(TRUE);
}


void SettingCommonPage::OnNMCustomdrawSliderImgSize(NMHDR *pNMHDR,LRESULT *pResult){
    LPNMCUSTOMDRAW pNMCD=reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    static int last=0;
    int now=sliderImgSize->GetPos();
    if(last!=now){
        setText(txtImgSize,MyString::Format("%.2lf%% %d×%d",now/0.16,now*10,now*10));
        if(last!=0){
            btnApply->EnableWindow(TRUE);
        }
        last=now;
    }
    *pResult=0;
}
