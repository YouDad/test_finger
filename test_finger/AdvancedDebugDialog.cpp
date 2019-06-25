#include "stdafx.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(AdvancedDebugDialog,CDialogEx)

// 构造函数
AdvancedDebugDialog::AdvancedDebugDialog(CWnd* pParent) : CDialogEx(IDD_ADVANCED_DEBUG_DIALOG,pParent){}

// 析构函数
AdvancedDebugDialog::~AdvancedDebugDialog(){}

// 防止Enter关闭窗口
void AdvancedDebugDialog::OnOK(){}

// 直方图组件
class MyControls{
public:
    // 原始图像
    CStatic* img;
    // 对应的亮度直方图
    CStatic* histogram;
    // 标明是第几张
    CStatic* id;
    // 显示时间
    CStatic* time;
    // 其他信息
    CStatic* hint;
    MyControls(int text,int i,CWnd* parent){
        // new出来
        img=new CStatic();
        histogram=new CStatic();
        id=new CStatic();
        time=new CStatic();
        hint=new CStatic();
        // 计算位置
        int top=10+80*i;
        int bottom=74+80*i;
        int textL=80+256+10;
        int textR=80+256+10+80;
        // 创建视图
        img->Create(0,WS_VISIBLE|WS_CHILD|WS_BORDER,CRect(10,top,74,bottom),parent);
        histogram->Create(0,WS_VISIBLE|WS_CHILD|WS_BORDER,CRect(80,top,80+256,bottom),parent);
        id->Create(MyString::Format("%d",text),WS_VISIBLE|WS_CHILD,CRect(textL,top,textR,top+20),parent);
        time->Create(MyString::Time("%H:%M:%S"),WS_VISIBLE|WS_CHILD,CRect(textL,top+20,textR,top+40),parent);
        hint->Create(MyString(""),WS_VISIBLE|WS_CHILD,CRect(textL,top+40,textR,top+60),parent);
    }
    void loadImage(){
        // 载入图像
        ::loadImage(img,MyFile::TEMP_IMAGE_PATH+"0.bmp");
        ::loadImage(histogram,MyFile::TEMP_IMAGE_PATH+"1.bmp");
    }
    void Destruction(){
        // 析构处理
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

// 组件向量
std::vector<MyControls>v;
// 支持上下滚动的变量
int position;

BOOL AdvancedDebugDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    position=0;
    return 0;
}

// 追加一个直方图组件
void AdvancedDebugDialog::append(){
    if(this){
        v.push_back(MyControls(v.size()+1,v.size()+1,this));
        v.back().loadImage();
        for(uint i=0;i<v.size();i++){
            setText(v[i].hint,MyString::Format("(%d/%d)",i+1,v.size()));
        }
    }
}

// 消息映射
BEGIN_MESSAGE_MAP(AdvancedDebugDialog,CDialogEx)
    ON_BN_CLICKED(IDC_BTNAD_ClearAll,&AdvancedDebugDialog::OnBnClickedBtnClearAll)
END_MESSAGE_MAP()

// 把所有直方图删除
void AdvancedDebugDialog::OnBnClickedBtnClearAll(){
    for(auto it=v.begin();it!=v.end();it++){
        it->Destruction();
    }
    v.clear();
    position=0;
}

// 向上滑动
void up(CWnd* c){
    if(position>0){
        position--;
        ScrollWindow(c->m_hWnd,0,80,0,0);
    }
}

// 向下滑动
void down(CWnd* c){
    if((uint)(position+6)<v.size()){
        position++;
        ScrollWindow(c->m_hWnd,0,-80,0,0);
    }
}

// 捕捉底层事件
BOOL AdvancedDebugDialog::PreTranslateMessage(MSG * pMsg){
    // 键盘按键弹起事件
    if(pMsg->message==WM_KEYUP){
        switch(pMsg->wParam){
            case VK_UP:up(this);break;
            case VK_DOWN:down(this);break;
            default:
                break;
        }
    }
    // 鼠标滚轮事件
    if(pMsg->message==WM_MOUSEWHEEL){
        if((int)pMsg->wParam>0){
            up(this);
        } else{
            down(this);
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
