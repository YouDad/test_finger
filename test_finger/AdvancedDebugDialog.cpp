#include "stdafx.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(AdvancedDebugDialog,CDialogEx)

AdvancedDebugDialog::AdvancedDebugDialog(CWnd* pParent)
    : CDialogEx(IDD_ADVANCED_DEBUG_DIALOG,pParent){}

AdvancedDebugDialog::~AdvancedDebugDialog(){}

void AdvancedDebugDialog::OnOK(){}

class MyControls{
public:
    CStatic* img;
    CStatic* histogram;
    CStatic* id;
    CStatic* time;
    CStatic* hint;
    MyControls(int text,int i,CWnd* parent){
        img=new CStatic();
        histogram=new CStatic();
        id=new CStatic();
        time=new CStatic();
        hint=new CStatic();
        int top=10+80*i;
        int bottom=74+80*i;
        int textL=80+256+10;
        int textR=80+256+10+80;
        img->Create(0,WS_VISIBLE|WS_CHILD|WS_BORDER,CRect(10,top,74,bottom),parent);
        histogram->Create(0,WS_VISIBLE|WS_CHILD|WS_BORDER,CRect(80,top,80+256,bottom),parent);
        id->Create(MyString::Format("%d",text),WS_VISIBLE|WS_CHILD,CRect(textL,top,textR,top+20),parent);
        time->Create(MyString::Time("%H:%M:%S"),WS_VISIBLE|WS_CHILD,CRect(textL,top+20,textR,top+40),parent);
        hint->Create(MyString(""),WS_VISIBLE|WS_CHILD,CRect(textL,top+40,textR,top+60),parent);
    }
    void loadImage(MyString id){
        ::loadImage(img,MyString("collectedTempImage/")+id+".bmp");
        ::loadImage(histogram,MyString("collectedTempImage/Histogram")+id+".bmp");
    }
    void Destruction(){
        img->DestroyWindow();
        delete img;
        img=0;
        histogram->DestroyWindow();
        delete histogram;
        histogram=0;
        id->DestroyWindow();
        delete id;
        id=0;
        time->DestroyWindow();
        delete time;
        time=0;
        hint->DestroyWindow();
        delete hint;
        hint=0;
    }
};

std::vector<MyControls>v;
int position;

BOOL AdvancedDebugDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    position=0;
    return 0;
}

void AdvancedDebugDialog::append(int id,MyString name){
    if(this){
        v.push_back(MyControls(id,v.size()+1,this));
        v.back().loadImage(name);
        for(int i=0;i<v.size();i++){
            setText(v[i].hint,MyString::Format("(%d/%d)",i+1,v.size()));
        }
    }
}

BEGIN_MESSAGE_MAP(AdvancedDebugDialog,CDialogEx)
    ON_BN_CLICKED(IDC_BTNAD_ClearAll,&AdvancedDebugDialog::OnBnClickedBtnadClearall)
END_MESSAGE_MAP()



void AdvancedDebugDialog::OnBnClickedBtnadClearall(){
    for(auto it=v.begin();it!=v.end();it++){
        it->Destruction();
    }
    v.clear();
    position=0;
}

void up(CWnd* c){
    if(position>0){
        position--;
        ScrollWindow(c->m_hWnd,0,80,0,0);
    }
}

void down(CWnd* c){
    if(position+6<v.size()){
        position++;
        ScrollWindow(c->m_hWnd,0,-80,0,0);
    }
}



BOOL AdvancedDebugDialog::PreTranslateMessage(MSG * pMsg){
    if(pMsg->message==WM_KEYUP){
        switch(pMsg->wParam){
            case VK_UP:up(this);break;
            case VK_DOWN:down(this);break;
            default:
                break;
        }
    }
    if(pMsg->message==WM_MOUSEWHEEL){
        if((int)pMsg->wParam>0){
            up(this);
        } else{
            down(this);
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
