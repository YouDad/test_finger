#include "stdafx.h"

IMPLEMENT_DYNAMIC(TempDialog,CDialogEx)

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
    setText(GetDlgItem(IDC_EDITBufferID),"1");
    return 0;
}

// 阻止Enter关闭窗口
void TempDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(TempDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
    ON_BN_CLICKED(IDC_BTNDownImage,&TempDialog::OnBnClickedBtnDownImage)
    ON_BN_CLICKED(IDC_BTNWriteNotepad,&TempDialog::OnBnClickedBtnWriteNotepad)
    ON_BN_CLICKED(IDC_BTNDownChar,&TempDialog::OnBnClickedBtnDownChar)
END_MESSAGE_MAP()

void TempDialog::OnBnClickedBtnDownImage(){
    MyString SelectedImagePath;
    if(!MyFile::OpenFileDialog("bmp",this,SelectedImagePath)){
        MyLog::error("未选择图片");
        return;
    }

    if(MyFile::ReadImage(SelectedImagePath,tempCommDataPacket)){
        isFreeRequest=2;
        comm.request(0x0b);
        MyLog::user("发送图像成功!");
    } else{
        MyLog::error("所选图像不能用于发送");
    }
}


void TempDialog::OnBnClickedBtnWriteNotepad(){
    MyString SelectedNotepadPath;
    if(!MyFile::OpenFileDialog("txt",this,SelectedNotepadPath)){
        MyLog::error("未选择记事本");
        return;
    }

    int NotepadID=MyString::AutoParseInt(getText(GetDlgItem(IDC_EDITNotepadID)));
    if(0>NotepadID||NotepadID>15){
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
        MyLog::user("发送Notepad成功!");
    } else{
        MyLog::error("该记事本格式不正确:%s",(const char*)msg);
    }
}

void TempDialog::OnBnClickedBtnDownChar(){
    MyString SelectedCharPath;
    if(!MyFile::OpenFileDialog("char",this,SelectedCharPath)){
        MyLog::error("未选择特征文件");
        return;
    }

    uint8_t BufferID=MyString::AutoParseInt(getText(GetDlgItem(IDC_EDITBufferID)));
    DataPacket data(&BufferID,1);

    if(MyFile::LoadCharFile(SelectedCharPath,tempCommDataPacket)){
        isFreeRequest=2;
        comm.request(0x09,data);
        MyLog::user("发送特征文件成功!");
    } else{
        MyLog::error("所选特征文件不能用于发送");
    }
}
