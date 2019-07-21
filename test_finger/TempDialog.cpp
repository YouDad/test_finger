#include "stdafx.h"

IMPLEMENT_DYNAMIC(TempDialog,CDialogEx)

CButton* btnSelectImage;
CButton* btnSelectNotepad;
CEdit* editNotepadID;

// 构造函数
TempDialog::TempDialog(CWnd* pParent)
    : CDialogEx(IDD_TempDialog,pParent){}

// 析构函数
TempDialog::~TempDialog(){
    //TODO 析构
}

LRESULT TempDialog::response(WPARAM w,LPARAM l){

    return TRUE;
}

// 初始化事件
BOOL TempDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();

    btnSelectImage=(CButton*)GetDlgItem(IDC_BTNSelectImage);
    btnSelectNotepad=(CButton*)GetDlgItem(IDC_BTNSelectNotepad);
    editNotepadID=(CEdit*)GetDlgItem(IDC_EDITNotepadID);

    return 0;
}

// 阻止Enter关闭窗口
void TempDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(TempDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
    ON_BN_CLICKED(IDC_BTNDownImage,&TempDialog::OnBnClickedBtnDownImage)
    ON_BN_CLICKED(IDC_BTNWriteNotepad,&TempDialog::OnBnClickedBtnWriteNotepad)
    ON_BN_CLICKED(IDC_BTNSelectImage,&TempDialog::OnBnClickedBtnSelectImage)
    ON_BN_CLICKED(IDC_BTNSelectNotepad,&TempDialog::OnBnClickedBtnSelectNotepad)
END_MESSAGE_MAP()


MyString SelectedImagePath="";
MyString SelectedNotepadPath="";

void Refresh(){
    if(SelectedImagePath==""){
        setText(btnSelectImage,"Select Image");
    } else{
        setText(btnSelectImage,"Selected!");
    }
    if(SelectedNotepadPath==""){
        setText(btnSelectNotepad,"Select Notepad");
    } else{
        setText(btnSelectNotepad,"Selected!");
    }
}

void TempDialog::OnBnClickedBtnDownImage(){
    if(SelectedImagePath!=""){
        if(MyFile::ReadImage(SelectedImagePath,tempCommDataPacket)){
            isFreeRequest=2;
            comm.request(0x0b);
        } else{
            MyLog::error("所选图像不能用于发送");
        }

        SelectedImagePath="";
        Refresh();
    } else{
        MyLog::error("未选择图片");
    }
}


void TempDialog::OnBnClickedBtnWriteNotepad(){
    if(SelectedNotepadPath==""){
        MyLog::error("未选择记事本");
        return;
    }
    int NotepadID=MyString::ParseInt(getText(editNotepadID));
    if(NotepadID<0||15<NotepadID){
        MyLog::error("NotepadID只能在[0,15]的范围内");
        return;
    }
    DataPacket data;
    MyString msg="";
    auto f=[&](FILE* fp)->bool{
        uint8_t pData[33];
        int ret,t;
        for(int i=0;i<32;i++){
            ret=fscanf(fp,"%x",&t);
            pData[i+1]=t;
            if(t<0||t>255){
                msg=MyString::Format("第%d个十六进制数未在[0x00,0xFF]范围内",i);
                return false;
            }
            if(ret==EOF){
                msg="十六进制数个数不足";
                return false;
            }
        }
        ret=fscanf(fp,"%*x");
        if(ret!=EOF){
            msg="十六进制数个数超过32个";
            return false;
        } else{
            pData[0]=NotepadID;
            data=DataPacket(pData,33);
            return true;
        }
    };
    if(MyFile::Read(SelectedNotepadPath,f)){
        isFreeRequest=2;
        comm.request(0x18,data);
    } else{
        MyLog::error("该记事本格式不正确:%s",(const char*)msg);
    }

    SelectedNotepadPath="";
    Refresh();
}


void TempDialog::OnBnClickedBtnSelectImage(){
    MyFile::OpenFileDialog("bmp",this,SelectedImagePath);
    Refresh();
}


void TempDialog::OnBnClickedBtnSelectNotepad(){
    MyFile::OpenFileDialog("txt",this,SelectedNotepadPath);
    Refresh();
}
