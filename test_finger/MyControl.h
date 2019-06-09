#pragma once
#include "MyString.h"
#include <afxwin.h>

MyString getText(CWnd* pWnd);
void setText(CWnd* pWnd,MyString str);

int getInt(CWnd* pWnd);
int getHex(CWnd* pWnd);
