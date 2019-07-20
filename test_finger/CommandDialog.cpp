#include "stdafx.h"

IMPLEMENT_DYNAMIC(CommandDialog,CDialogEx)

// 构造函数
CommandDialog::CommandDialog(std::vector<struct Command>& v,CWnd* pParent)
    : CDialogEx(IDD_CommandDialog,pParent){
    commands=v;
}

// 析构函数
CommandDialog::~CommandDialog(){
    //TODO 析构
}

LRESULT CommandDialog::response(WPARAM w,LPARAM l){

    return TRUE;
}

class CommandControl{
public:
    CommandControl(int height,struct Command c,CWnd* parent){
        CmdCode=c.CmdCode;
        btn=new CButton();
        btn->Create(c.Name,0,CRect(5,height,125,height+25),parent,nID);
        btn->SetFont(defaultFont);
        btn->ShowWindow(SW_SHOW);
        for(int i=0;i<c.DfaultValues.size();i++){
            edt.push_back(new CEdit());
            edt[i]->Create(WS_BORDER,CRect(130+45*i,height,170+45*i,height+25),parent,nID*10+i);
            edt[i]->SetFont(defaultFont);
            edt[i]->ShowWindow(SW_SHOW);
            setText(edt[i],MyString::IntToMyString(c.DfaultValues[i]));
        }
        nID++;
    }
    DataPacket Click(){
        std::vector<uint8_t> v;
        v.push_back((uint8_t)CmdCode);
        for(int i=0;i<edt.size();i++){
            int a,b;
            MyString txt=getText(edt[i]);
            txt.Parse("%d",&a);
            txt.Parse("%x",&b);
            a=max(a,b);
            if(0>a&&a>255){
                return DataPacket();
            }
            v.push_back(a);
        }
        return DataPacket(v);
    }
    bool isFocused(){
        return btn==btn->GetParent()->GetFocus();
    }
private:
    static int nID;
    int CmdCode;
    CButton* btn;
    std::vector<CEdit*> edt;
};
int CommandControl::nID=2000;
CommandControl* cc[10];
// 初始化事件
BOOL CommandDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();

    for(int i=0;i<min(10,commands.size());i++){
        cc[i]=new CommandControl(5+i*30,commands[i],this);
    }
    return 0;
}

// 阻止Enter关闭窗口
void CommandDialog::OnOK(){}

// 消息映射
BEGIN_MESSAGE_MAP(CommandDialog,CDialogEx)
    RESPONSE_USER_MESSAGE(response)
    ON_BN_CLICKED(2000,&CommandDialog::OnClick)
    ON_BN_CLICKED(2001,&CommandDialog::OnClick)
    ON_BN_CLICKED(2002,&CommandDialog::OnClick)
    ON_BN_CLICKED(2003,&CommandDialog::OnClick)
    ON_BN_CLICKED(2004,&CommandDialog::OnClick)
    ON_BN_CLICKED(2005,&CommandDialog::OnClick)
    ON_BN_CLICKED(2006,&CommandDialog::OnClick)
    ON_BN_CLICKED(2007,&CommandDialog::OnClick)
    ON_BN_CLICKED(2008,&CommandDialog::OnClick)
    ON_BN_CLICKED(2009,&CommandDialog::OnClick)
END_MESSAGE_MAP()

int isFreeRequest=0;

void CommandDialog::OnClick(){
    for(int i=0;cc[i];i++){
        if(cc[i]->isFocused()){
            DataPacket data=cc[i]->Click();
            if(data.isValid()){
                int cmdcode=data.getPointer()[0];
                data.readData(1);
                if(comm.getConnectId()<0){
                    MyLog::user("还未连接");
                    return;
                }
                isFreeRequest=2;
                comm.request(cmdcode,data);
            } else{
                MyLog::user("参数有误");
                return;
            }
        }
    }
}
