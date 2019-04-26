#pragma once
#include"stdafx.h"

char* CString2char(CString&c){
	static char text[65536+10];
	int len=WideCharToMultiByte(0,0,c,c.GetLength(),0,0,0,0);
	WideCharToMultiByte(0,0,c,c.GetLength(),text,len,0,0);
	text[len]=0;
	return text;
}

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
	time_t curtime;
	time(&curtime);
	sprintf(tstr,"%s ",ctime(&curtime));
	return tstr;
}

void log(int level,CString info){
	if(level>LOGT){
		ASF_WARNING(1);
	}
	if(level>cmbLogLevel->GetCurSel())return;
	CString time(timeStr()),old;
	int len=editLog->GetWindowTextLength();
	//选定当前文本的末端
	editLog->SetSel(len,len);
	//追加文本
	editLog->ReplaceSel(time+info+"\r\n");

	//自动备份
	editLog->GetWindowText(old);
	char* text=CString2char(old);
	//write into file
	FILE* fp=fopen("backup.txt","w");
	fprintf(fp,"%s",text);
	fclose(fp);
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
		disable(cmbChipType);
		disable(btnRawImage);
		disable(btnTestImage);
		disable(btnContinueImage);
		disable(btnContinueBackGroundImage);
		disable(btnSetCmos);
		disable(btnSetBaud);
		disable(btnSetPassword);
		disable(btnSetAddress);
		disable(btnReadReg);
		disable(btnWriteReg);
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
		enable(cmbChipType);
		enable(btnRawImage);
		enable(btnTestImage);
		enable(btnContinueImage);
		enable(btnContinueBackGroundImage);
		enable(btnSetCmos);
		enable(btnSetBaud);
		enable(btnSetPassword);
		enable(btnSetAddress);
		enable(btnReadReg);
		enable(btnWriteReg);

		disable(cmbBaud);
		disable(cmbWay);
		break;
	case actClosedPort:
		enable(btnConnect);
		goto actInit;
		break;
	case actReadingReg:
	case actWritingReg:
	case actGetingImage:
		disable(btnConnect);
		disable(editAddress);
		disable(editPassword);
		disable(editAddressSet);
		disable(editPasswordSet);
		disable(editLightTime);
		disable(editSensitivity);
		disable(cmbBaudSet);
		disable(cmbChipType);
		disable(btnRawImage);
		disable(btnTestImage);
		disable(btnSetCmos);
		disable(btnSetBaud);
		disable(btnSetPassword);
		disable(btnSetAddress);
		disable(btnContinueImage);
		disable(btnContinueBackGroundImage);
		disable(btnReadReg);
		disable(btnWriteReg);
		break;
	case actReadedReg:
	case actWritedReg:
	case actGotImage:
		enable(btnConnect);
		enable(editAddress);
		enable(editPassword);
		enable(editAddressSet);
		enable(editPasswordSet);
		enable(editLightTime);
		enable(editSensitivity);
		enable(cmbBaudSet);
		enable(cmbChipType);
		enable(btnRawImage);
		enable(btnTestImage);
		enable(btnContinueImage);
		enable(btnContinueBackGroundImage);
		enable(btnSetCmos);
		enable(btnSetBaud);
		enable(btnSetPassword);
		enable(btnSetAddress);
		enable(btnReadReg);
		enable(btnWriteReg);
		break;
	case actGetConBKI:
	case actGetConImage:
		if(a==actGetConBKI){
			disable(btnContinueImage);
			enable(btnContinueBackGroundImage);
		}else{
			enable(btnContinueImage);
			disable(btnContinueBackGroundImage);
		}
		
		disable(editAddress);
		disable(editPassword);
		disable(editAddressSet);
		disable(editPasswordSet);
		disable(editLightTime);
		disable(editSensitivity);
		disable(cmbBaudSet);
		disable(cmbChipType);
		disable(btnRawImage);
		disable(btnTestImage);
		disable(btnSetCmos);
		disable(btnSetBaud);
		disable(btnSetPassword);
		disable(btnSetAddress);
		disable(btnReadReg);
		disable(btnWriteReg);
		disable(btnConnect);
		break;
	case actStpGetBKI:
	case actStpGetImage:
		enable(btnConnect);
		enable(editAddress);
		enable(editPassword);
		enable(editAddressSet);
		enable(editPasswordSet);
		enable(editLightTime);
		enable(editSensitivity);
		enable(cmbBaudSet);
		enable(cmbChipType);
		enable(btnRawImage);
		enable(btnTestImage);
		enable(btnContinueImage);
		enable(btnContinueBackGroundImage);
		enable(btnSetCmos);
		enable(btnSetBaud);
		enable(btnSetPassword);
		enable(btnSetAddress);
		enable(btnReadReg);
		enable(btnWriteReg);
		break;
	}
}

void loadImage(WCHAR* filePath){
	HBITMAP hBmp=(HBITMAP)LoadImage(0,filePath,0,0,0,LR_LOADFROMFILE);
	if(hBmp){
		image->ModifyStyle(0xf,SS_BITMAP|SS_CENTERIMAGE);
		image->SetBitmap(hBmp);
	}else{
		ASF_WARNING(4);
	}
}

bool saveBmp(int h,int w,BYTE*pData,CString dir,CString path){
    for(int i=0;i<w*h-1-i;i++){
        BYTE t = pData[w*h-1-i];
        pData[w*h-1-i]=pData[i];
        pData[i]=t;
    }
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
	
	CreateDirectory(dir,0);

	char* filePath=CString2char(path);
	FILE* fp=fopen(filePath,"wb");
	if(fp==NULL){
		ASF_ERROR(1);
		return false;
	}
	fwrite(&bmpFileInfo,sizeof bmpFileInfo,1,fp);
	fwrite(&bmpInfo,sizeof bmpInfo,1,fp);
	fwrite(&bmfColorQuad,sizeof bmfColorQuad,1,fp);
	fwrite(pData,w*h,1,fp);
	fclose(fp);
	return true;
}

void saveImage(CString x){
	CString path=CTime::GetCurrentTime().Format("%Y_%m_%d_%H_%M_%S");
	path=x+_T("/")+path+_T(".bmp");
	//if(packetDataLen!=160*160)
	//	ASF_WARNING(3);


	int w,h;
	if(packetDataLen==160*160){
		log(LOGU,"接收到160x160的图像");
		w=h=160;
		saveBmp(w,h,packetData,x,path);
		loadImage((LPTSTR)(LPCTSTR)path);
	}else if(packetDataLen==192*192){
		log(LOGU,"接收到192x192的图像");
		w=h=192;
		saveBmp(w,h,packetData,x,path);
		loadImage((LPTSTR)(LPCTSTR)path);
	}else{
		log(LOGU,"既不是160x160也不是192x192,没法渲染图像");
	}
}