#pragma once
#include "stdafx.h"

int NetGetVersion();
void NetDownload(CProgressCtrl* process,CStatic* detail);
bool isConnectedNet();