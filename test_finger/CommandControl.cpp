#include "stdafx.h"

CommandControl::CommandControl(int height,struct Command c,CWnd * parent){
    CmdCode=c.CmdCode;
    btn=new CButton();
    btn->Create(c.Name,0,CRect(5,height,125,height+25),parent,nID);
    btn->SetFont(defaultFont);
    btn->ShowWindow(SW_SHOW);
    for(int i=0;i<c.DefaultValues.size();i++){
        edt.push_back(new CEdit());
        edt[i]->Create(WS_BORDER,CRect(130+45*i,height,170+45*i,height+25),parent,nID*10+i);
        edt[i]->SetFont(defaultFont);
        edt[i]->ShowWindow(SW_SHOW);
        setText(edt[i],MyString::IntToMyString(c.DefaultValues[i]));
    }
    nID++;
}

DataPacket CommandControl::Click(){
    std::vector<uint8_t> v;
    v.push_back((uint8_t)CmdCode);
    for(int i=0;i<edt.size();i++){
        int a=MyString::AutoParseInt(getText(edt[i]));
        if(0>a||a>255){
            return DataPacket();
        }
        v.push_back(a);
    }
    return DataPacket(v);
}

bool CommandControl::isFocused(){
    return btn==btn->GetParent()->GetFocus();
}
