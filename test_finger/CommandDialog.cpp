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

int CommandControl::nID=2000;

// 初始化事件
BOOL CommandDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();

    memset(cc,0,sizeof cc);
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
    ON_BN_CLICKED(2010,&CommandDialog::OnClick)
    ON_BN_CLICKED(2011,&CommandDialog::OnClick)
    ON_BN_CLICKED(2012,&CommandDialog::OnClick)
    ON_BN_CLICKED(2013,&CommandDialog::OnClick)
    ON_BN_CLICKED(2014,&CommandDialog::OnClick)
    ON_BN_CLICKED(2015,&CommandDialog::OnClick)
    ON_BN_CLICKED(2016,&CommandDialog::OnClick)
    ON_BN_CLICKED(2017,&CommandDialog::OnClick)
    ON_BN_CLICKED(2018,&CommandDialog::OnClick)
    ON_BN_CLICKED(2019,&CommandDialog::OnClick)
    ON_BN_CLICKED(2020,&CommandDialog::OnClick)
    ON_BN_CLICKED(2021,&CommandDialog::OnClick)
    ON_BN_CLICKED(2022,&CommandDialog::OnClick)
    ON_BN_CLICKED(2023,&CommandDialog::OnClick)
    ON_BN_CLICKED(2024,&CommandDialog::OnClick)
    ON_BN_CLICKED(2025,&CommandDialog::OnClick)
    ON_BN_CLICKED(2026,&CommandDialog::OnClick)
    ON_BN_CLICKED(2027,&CommandDialog::OnClick)
    ON_BN_CLICKED(2028,&CommandDialog::OnClick)
    ON_BN_CLICKED(2029,&CommandDialog::OnClick)
    ON_BN_CLICKED(2030,&CommandDialog::OnClick)
    ON_BN_CLICKED(2031,&CommandDialog::OnClick)
    ON_BN_CLICKED(2032,&CommandDialog::OnClick)
    ON_BN_CLICKED(2033,&CommandDialog::OnClick)
    ON_BN_CLICKED(2034,&CommandDialog::OnClick)
    ON_BN_CLICKED(2035,&CommandDialog::OnClick)
    ON_BN_CLICKED(2036,&CommandDialog::OnClick)
    ON_BN_CLICKED(2037,&CommandDialog::OnClick)
    ON_BN_CLICKED(2038,&CommandDialog::OnClick)
    ON_BN_CLICKED(2039,&CommandDialog::OnClick)
    ON_BN_CLICKED(2040,&CommandDialog::OnClick)
    ON_BN_CLICKED(2041,&CommandDialog::OnClick)
    ON_BN_CLICKED(2042,&CommandDialog::OnClick)
    ON_BN_CLICKED(2043,&CommandDialog::OnClick)
    ON_BN_CLICKED(2044,&CommandDialog::OnClick)
    ON_BN_CLICKED(2045,&CommandDialog::OnClick)
    ON_BN_CLICKED(2046,&CommandDialog::OnClick)
    ON_BN_CLICKED(2047,&CommandDialog::OnClick)
    ON_BN_CLICKED(2048,&CommandDialog::OnClick)
    ON_BN_CLICKED(2049,&CommandDialog::OnClick)
END_MESSAGE_MAP()

int isFreeRequest=0;

void CommandDialog::OnClick(){
    for(int i=0;cc[i]&&i<10;i++){
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
