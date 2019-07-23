#pragma once
#include "DataPacket.h"
#include "MainDialog.h"
#include "MainControl.h"

struct Command{
    MyString Name;
    int CmdCode;
    std::vector<uint8_t> DfaultValues;
};

class CommandControl{
public:
    CommandControl(int height,struct Command c,CWnd* parent){
        CmdCode=c.CmdCode;
        NEW_INFO;
        btn=new CButton();
        btn->Create(c.Name,0,CRect(5,height,125,height+25),parent,nID);
        btn->SetFont(defaultFont);
        btn->ShowWindow(SW_SHOW);
        for(int i=0;i<c.DfaultValues.size();i++){
            NEW_INFO;
            edt.push_back(new CEdit());
            edt[i]->Create(WS_BORDER,CRect(130+45*i,height,170+45*i,height+25),parent,nID*10+i);
            edt[i]->SetFont(defaultFont);
            edt[i]->ShowWindow(SW_SHOW);
            setText(edt[i],MyString::IntToMyString(c.DfaultValues[i]));
        }
        nID++;
    }
    ~CommandControl(){
        DEL_INFO;
        delete btn;
        for(int i=0;i<edt.size();i++){
            DEL_INFO;
            delete edt[i];
        }
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
        this;
        return btn==btn->GetParent()->GetFocus();
    }
private:
    static int nID;
    int CmdCode;
    CButton* btn;
    std::vector<CEdit*> edt;
};

class CommandDialog: public CDialogEx{
    DECLARE_DYNAMIC(CommandDialog)

public:
    CommandDialog(std::vector<struct Command>& v,CWnd* pParent=nullptr);
    virtual ~CommandDialog();
    //消息处理函数
    afx_msg LRESULT response(WPARAM w,LPARAM l);

#ifdef AFX_DESIGN_TIME
    enum{
        IDD=IDD_TabsDialog
    };
#endif

protected:
    std::vector<struct Command> commands;
    virtual BOOL OnInitDialog();
    DECLARE_MESSAGE_MAP()
    CommandControl* cc[10];

public:
    virtual void OnOK();
    afx_msg void OnClick();
};
extern int isFreeRequest;
