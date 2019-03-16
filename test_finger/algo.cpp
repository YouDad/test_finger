#pragma once
#include"stdafx.h"
#include"algo.h"
CSerial serial;//串口对象

void enumerateSerialPorts(CUIntArray& exist,CUIntArray& useful,CUIntArray& used){
    //清除串口数组内容
    exist.RemoveAll();
    useful.RemoveAll();
    used.RemoveAll();
    int portState=0;
    //因为至多有255个串口，所以依次检查各串口是否存在
    //如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，
    //都认为串口存在，只不过后者表明串口已经被占用,否则串口不存在
	//检查255个太费时间,且用前30个的几率较大
    for(int i=1; i<30; i++){
        //生成原始设备名
        CString sPort;
        sPort.Format(_T("\\\\.\\COM%d"),i);

        //试图打开此串口
        BOOL bSuccess;
        HANDLE hPort=::CreateFile(sPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
        if(hPort==INVALID_HANDLE_VALUE){
			//打开串口失败
			bSuccess=FALSE;
            DWORD dwError=GetLastError();
            if(dwError==ERROR_ACCESS_DENIED){
                bSuccess=TRUE;
                used.Add(i);//已占用的串口
            }
        } else{
            //打开串口成功
            bSuccess=TRUE;
            useful.Add(i);//可用的串口
            if(!CloseHandle(hPort)){
                DWORD dwError=GetLastError();//关闭不成功的原因
            }
        }
        if(bSuccess){
            exist.Add(i);//所有存在的串口
        }
    }
}

void updateCommunityWay(){
	CUIntArray exist,useful,used;
	CString name;
	cmbWay->ResetContent();
	enumerateSerialPorts(exist,useful,used);
	if(exist.GetSize()>0){
		for(int i=0;i<exist.GetSize();i++){
			name.Format(_T("COM%d"),exist.ElementAt(i));
			cmbWay->InsertString(i,name);
		}
		cmbWay->SetCurSel(0);
		log(LOGU,"发现%d个串口",exist.GetSize());
	}else{
		log(LOGU,"未发现存在的串口");
	}
}

char* timeStr(){
	static char tstr[64];
	time_t tt=time(NULL);//返回一个时间戳
	tm* t=localtime(&tt);
	sprintf(tstr,"%d-%02d-%02d %02d:%02d:%02d ",
		t->tm_year+1900,
		t->tm_mon+1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	return tstr;
}

void log(int level,CString info){
	if(level>cmbLogLevel->GetCurSel())return;
	CString time(timeStr()),old;
	editLog->GetWindowText(old);
	editLog->SetWindowText(old+time+info+"\r\n");
	editLog->SendMessage(WM_VSCROLL,SB_BOTTOM,0);//滚动条始终在底部
}

void log(int level,const char* format,...){
	static char tmp[512];
	va_list ap;
	va_start(ap,format);
	vsprintf(tmp,format,ap);
	va_end(ap);
	
	CString info(tmp);
	log(level,info);
}

//本地函数,禁用控件
void disable(CWnd* pWnd){
	pWnd->EnableWindow(FALSE);
}
//本地函数,启用控件
void enable(CWnd* pWnd){
	pWnd->EnableWindow(TRUE);
}

void updateControlDisable(action a){
	switch(a){
	case actInit:
actInit://一个标签
		enable(cmbBaud);
		enable(cmbWay);

		disable(editAddress);
		disable(editPassword);
		disable(editAddressSet);
		disable(editPasswordSet);
		disable(editLightTime);
		disable(editSensitivity);
		disable(cmbBaudSet);
		disable(cmbSecurity);
		disable(btnReset);
		disable(btnRawImage);
		disable(btnContinueImage);
		disable(btnSetSecurity);
		disable(btnSetCmos);
		disable(btnSetBaud);
		disable(btnSetPassword);
		disable(btnSetAddress);
		break;
	case actOpeningPort:
	case actClosingPort:
		disable(cmbBaud);
		disable(cmbWay);
		disable(btnConnect);
		break;
	case actOpenedPort:
		enable(btnConnect);
		enable(editAddress);
		enable(editPassword);
		enable(editAddressSet);
		enable(editPasswordSet);
		enable(editLightTime);
		enable(editSensitivity);
		enable(cmbBaudSet);
		enable(cmbSecurity);
		enable(btnReset);
		enable(btnRawImage);
		enable(btnContinueImage);
		enable(btnSetSecurity);
		enable(btnSetCmos);
		enable(btnSetBaud);
		enable(btnSetPassword);
		enable(btnSetAddress);

		disable(cmbBaud);
		disable(cmbWay);
		break;
	case actClosedPort:
		enable(btnConnect);
		goto actInit;
		break;
	case actGetConImage:
		enable(btnContinueImage);
		
		disable(btnConnect);
		disable(editAddress);
		disable(editPassword);
		disable(editAddressSet);
		disable(editPasswordSet);
		disable(editLightTime);
		disable(editSensitivity);
		disable(cmbBaudSet);
		disable(cmbSecurity);
		disable(btnReset);
		disable(btnRawImage);
		disable(btnSetSecurity);
		disable(btnSetCmos);
		disable(btnSetBaud);
		disable(btnSetPassword);
		disable(btnSetAddress);
		break;
	case actStpGetImage:
		enable(btnConnect);
		enable(editAddress);
		enable(editPassword);
		enable(editAddressSet);
		enable(editPasswordSet);
		enable(editLightTime);
		enable(editSensitivity);
		enable(cmbBaudSet);
		enable(cmbSecurity);
		enable(btnReset);
		enable(btnRawImage);
		enable(btnContinueImage);
		enable(btnSetSecurity);
		enable(btnSetCmos);
		enable(btnSetBaud);
		enable(btnSetPassword);
		enable(btnSetAddress);
		break;
	}
}

void loadImage(WCHAR* filePath){
    HBITMAP hBmp=(HBITMAP)LoadImage(0,filePath,0,0,0,LR_LOADFROMFILE);
	image->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
    image->SetBitmap(hBmp);
}

void saveBmp(int h,int w,BYTE*pData,CString path){
    HANDLE hFile;

    BITMAPINFOHEADER bmpInfo;
    bmpInfo.biSize=sizeof bmpInfo;
    bmpInfo.biWidth=w;
    bmpInfo.biHeight=h;
    bmpInfo.biPlanes=1;
    bmpInfo.biBitCount=8;
    bmpInfo.biCompression=BI_RGB;
    bmpInfo.biSizeImage=0;
    bmpInfo.biXPelsPerMeter=0;
    bmpInfo.biYPelsPerMeter=0;
    bmpInfo.biClrUsed=256;
    bmpInfo.biClrImportant=256;
    RGBQUAD bmfColorQuad[256];
    for(int i=0;i<256;i++){
        bmfColorQuad[i].rgbBlue=i;
        bmfColorQuad[i].rgbGreen=i;
        bmfColorQuad[i].rgbRed=i;
        bmfColorQuad[i].rgbReserved=0;
    }
    BITMAPFILEHEADER bmpFileInfo;
    bmpFileInfo.bfType=0x4d42;//"BM"
    bmpFileInfo.bfSize=0x400+w*h+(sizeof bmpInfo)+sizeof(bmpFileInfo);
    bmpFileInfo.bfReserved1=0;
    bmpFileInfo.bfReserved2=0;
    bmpFileInfo.bfOffBits=0x400+sizeof(BITMAPFILEHEADER)+
		sizeof(BITMAPINFOHEADER);

	CreateDirectory(_T("collectedImage"),0);
    hFile=CreateFile(path,GENERIC_READ|GENERIC_WRITE,0,
		NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(hFile==INVALID_HANDLE_VALUE){
        return;
    }
    WriteFile(hFile,&bmpFileInfo,sizeof bmpFileInfo,0,0);
    WriteFile(hFile,&bmpInfo,sizeof bmpInfo,0,0);
    WriteFile(hFile,&bmfColorQuad,sizeof bmfColorQuad,0,0);
	WriteFile(hFile,pData,w*h,0,0);
    CloseHandle(hFile);
}