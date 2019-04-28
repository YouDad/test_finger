#pragma once
#include"stdafx.h"

int CString2int(CString c){
    int ret;
    swscanf(c,_T("%d"),&ret);
    return ret;
}

char* CString2char(CString&c){
    static char text[65536+10];
    int len=WideCharToMultiByte(0,0,c,c.GetLength(),0,0,0,0);
    WideCharToMultiByte(0,0,c,c.GetLength(),text,len,0,0);
    text[len]=0;
    return text;
}

std::vector<int>* idle=new std::vector<int>();
std::vector<int>* lastIdle=NULL;

void enumerateSerialPorts(){
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
    if(lastIdle){
        delete lastIdle;
    }
    lastIdle=idle;
    idle=new std::vector<int>();

    enumerateSerialPorts();
    //UI:组合框 更新
    cmbWay->ResetContent();
    if(idle->size()==0){
        MyLog.print(Log::LOGU,"未发现空闲的串口");
        return;
    } else{
        MyLog.print(Log::LOGU,"发现%d个串口",idle->size());
        for(int i=0;i<idle->size();i++){
            CString name;
            name.Format(_T("COM%d"),(*idle)[i]);
            cmbWay->InsertString(i,name);
        }
    }
}


void autoConnect(){
    int id=comm.getConnectId();
    //自动连接只在未连接状态生效
    if(id==0){
        if(idle->size()==lastIdle->size()+1){ //应该会增加一个空闲设备
            std::vector<int> diff;
            std::set_difference(
                idle->begin(),idle->end(),
                lastIdle->begin(),lastIdle->end(),
                std::inserter(diff,diff.begin())
            );//求idle-lastIdle
            if(diff.size()==1){ //差集应该只有一个元素
                int needConnectId=diff[0];
                for(int i=0;i<idle->size();i++){
                    if((*idle)[i]==needConnectId){
                        cmbWay->SetCurSel(i);
                        CString baud;
                        cmbBaud->GetWindowTextW(baud);
                        bool ret=comm.connect(needConnectId,CString2int(baud));
                        if(ret){
                            btnConnect->SetWindowText(_T("断开连接"));
                            updateControlDisable(actOpenedPort);
                        } else{
                            updateControlDisable(actClosedPort);
                        }
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
    }
}

void autoDisconnect(){
    int id=comm.getConnectId();
    //自动断开仅当连接状态下
    if(id!=0){
        bool needDisconnect=true;
        for(int i=0;i<idle->size();i++){
            if((*idle)[i]==id){
                needDisconnect=false;
            }
        }
        if(needDisconnect){
            comm.disconnect();
            updateControlDisable(actClosedPort);
            btnConnect->SetWindowText(_T("连接下位机"));
        }
    }
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

void saveImage(CString x,DataPacket dataPacket){
    CString path=CTime::GetCurrentTime().Format("%Y_%m_%d_%H_%M_%S");
    path=x+_T("/")+path+_T(".bmp");

    int w,h;
    if(dataPacket.len==160*160){
        MyLog.print(Log::LOGU,"接收到160x160的图像");
        w=h=160;
        saveBmp(w,h,dataPacket.data,x,path);
        loadImage((LPTSTR)(LPCTSTR)path);
    } else if(dataPacket.len==192*192){
        MyLog.print(Log::LOGU,"接收到192x192的图像");
        w=h=192;
        saveBmp(w,h,dataPacket.data,x,path);
        loadImage((LPTSTR)(LPCTSTR)path);
    } else{
        MyLog.print(Log::LOGU,"既不是160x160也不是192x192,没法渲染图像");
    }
}

const uint16_t CRC16Table[256]={
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
//X16 + X12 + X5 + 1 余式表
uint16_t GetCRC16(const void*pSource,uint16_t len){
    uint16_t i;
    uint16_t result=0;
    uint8_t* p=(uint8_t*)pSource;
    for(i=0; i<len; i++){
        result=(result<<8)^CRC16Table[(result>>8)^*p++];
    }
    return result;
}