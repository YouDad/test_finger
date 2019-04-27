#pragma once
#include"stdafx.h"

char* CString2char(CString&c){
    static char text[65536+10];
    int len=WideCharToMultiByte(0,0,c,c.GetLength(),0,0,0,0);
    WideCharToMultiByte(0,0,c,c.GetLength(),text,len,0,0);
    text[len]=0;
    return text;
}

void enumerateSerialPorts(std::vector<int>* idle){
    //清除串口数组内容
    idle->clear();
    //因为至多有255个串口，所以依次检查各串口是否存在
    //如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，
    //都认为串口存在，只不过后者表明串口已经被占用,否则串口不存在
    //检查255个太费时间,且用前30个的几率较大
    for(int i=1; i<30; i++){
        //生成原始设备名
        CString sPort;
        sPort.Format(_T("\\\\.\\COM%d"),i);

        HANDLE hPort=::CreateFile(sPort,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
        if(hPort!=INVALID_HANDLE_VALUE){
            idle->push_back(i);
            CloseHandle(hPort);
        }
    }
}

void updateCommunityWay(){
    static std::vector<int>* idle=new std::vector<int>();
    static std::vector<int>* lastIdle=NULL;
    enumerateSerialPorts(idle);

    //UI:组合框 更新
    if(idle->size()==0){
        MyLog.print(Log::LOGU,"未发现空闲的串口");
        return;
    } else{
        MyLog.print(Log::LOGU,"发现%d个串口",idle->size());
        cmbWay->ResetContent();
        for(int i=0;i<idle->size();i++){
            CString name;
            name.Format(_T("COM%d"),(*idle)[i]);
            cmbWay->InsertString(i,name);
        }
    }

    //没有可比较的对象,第一次枚举
    if(lastIdle==NULL){
        lastIdle=idle;
        idle=new std::vector<int>();
        return;
    }

    int id=CCommunication::getConnectId();
    //处于未连接状态,可能有两种动作:1.直接插入触发这个事件 2.断开之后拔触发事件
    if(id==0){
        if(idle->size()==lastIdle->size()+1){
            //1.直接插入触发这个事件
            std::vector<int> diff;
            std::set_difference(
                idle->begin(),idle->end(),
                lastIdle->begin(),lastIdle->end(),
                std::inserter(diff,diff.begin())
            );//求idle-lastIdle
            if(diff.size()==1){
                int needConnectId=diff[0];
                for(int i=0;i<idle->size();i++){
                    if((*idle)[i]==needConnectId){
                        cmbWay->SetCurSel(i);
                    }
                }
            } else{
                MyLog.print(Log::LOGE,"发现Bug,当前串口处于未连接状态,经过一次串口枚举,发现idle和lastIdle的差集元素个数不是一个,详细信息:");
                {
                    CString error=_T("diff:");
                    for(std::vector<int>::iterator it=diff.begin();it!=diff.end();it++){
                        char number[20];
                        sprintf(number," %d",*it);
                        error+=number;
                    }
                    MyLog.print(Log::LOGE,error);
                }
            }
        } else if(idle->size()==lastIdle->size()-1){
            //2.断开之后拔触发事件
        } else{
            //其他异常情况
            MyLog.print(Log::LOGE,"发现Bug,当前串口处于未连接状态,经过一次串口枚举,发现idle并不比lastIdle多,详细信息:");
            {
                CString error=_T("idle:");
                for(std::vector<int>::iterator it=idle->begin();it!=idle->end();it++){
                    char number[20];
                    sprintf(number," %d",*it);
                    error+=number;
                }
                MyLog.print(Log::LOGE,error);
            }
            {
                CString error=_T("lastIdle:");
                for(std::vector<int>::iterator it=lastIdle->begin();it!=lastIdle->end();it++){
                    char number[20];
                    sprintf(number," %d",*it);
                    error+=number;
                }
                MyLog.print(Log::LOGE,error);
            }
        }
    } else{
        bool needDisconnect=true;
        for(int i=0;i<idle->size();i++){
            if((*idle)[i]==id){
                needDisconnect=false;
            }
        }
        if(needDisconnect){
            CCommunication::disConnect();
        }

    }
    lastIdle=idle;
    idle=new std::vector<int>();
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
            } else{
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
    } else{
        ASF_WARNING(4);
    }
}

bool saveBmp(int h,int w,BYTE*pData,CString dir,CString path){
    for(int i=0;i<w*h-1-i;i++){
        BYTE t=pData[w*h-1-i];
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

    int w,h;
    if(packetDataLen==160*160){
        MyLog.print(Log::LOGU,"接收到160x160的图像");
        w=h=160;
        saveBmp(w,h,packetData,x,path);
        loadImage((LPTSTR)(LPCTSTR)path);
    } else if(packetDataLen==192*192){
        MyLog.print(Log::LOGU,"接收到192x192的图像");
        w=h=192;
        saveBmp(w,h,packetData,x,path);
        loadImage((LPTSTR)(LPCTSTR)path);
    } else{
        MyLog.print(Log::LOGU,"既不是160x160也不是192x192,没法渲染图像");
    }
}