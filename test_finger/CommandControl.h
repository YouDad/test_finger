#pragma once
#include "MyString.h"
#include <vector>
#include "DataPacket.h"
#include <afxwin.h>

struct Command{
    MyString Name;
    int CmdCode;
    std::vector<uint8_t> DefaultValues;
};

class CommandControl{
public:
    CommandControl(int height,struct Command c,CWnd* parent);
    DataPacket Click();
    bool isFocused();
private:
    static int nID;
    int CmdCode;
    CButton* btn;
    std::vector<CEdit*> edt;
};