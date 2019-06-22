#include "stdafx.h"

// 把p代表的数组,长度为len翻转一下
void reverse(uint8_t* p,int len){
    for(int i=0;i<len-1-i;i++){
        uint8_t t=p[len-1-i];
        p[len-1-i]=p[i];
        p[i]=t;
    }
}

BITMAPINFOHEADER bmpInfo;
RGBQUAD bmfColorQuad[256];
BITMAPFILEHEADER bmpFileInfo;
bool isInit=false;

void ImageInit(int w,int h){
    if(!isInit){
        bmpInfo.biSize=sizeof bmpInfo;
        bmpInfo.biPlanes=1;
        bmpInfo.biBitCount=8;
        bmpInfo.biCompression=BI_RGB;
        bmpInfo.biSizeImage=0;
        bmpInfo.biXPelsPerMeter=0;
        bmpInfo.biYPelsPerMeter=0;
        bmpInfo.biClrUsed=256;
        bmpInfo.biClrImportant=256;
        for(int i=0;i<256;i++){
            bmfColorQuad[i].rgbBlue=i;
            bmfColorQuad[i].rgbGreen=i;
            bmfColorQuad[i].rgbRed=i;
            bmfColorQuad[i].rgbReserved=0;
        }
        bmpFileInfo.bfType=0x4d42;//"BM"
        bmpFileInfo.bfReserved1=0;
        bmpFileInfo.bfReserved2=0;
        bmpFileInfo.bfOffBits=0x400+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
    }
    bmpInfo.biWidth=w;
    bmpInfo.biHeight=h;
    bmpFileInfo.bfSize=0x400+w*h+(sizeof bmpInfo)+sizeof(bmpFileInfo);
}

bool saveTempImage(int w,int h,uint8_t* pData,int id){
    if(test.isTest(test.UI)){
        return true;
    }
    ImageInit(w,h);
    return MyFile::SaveTempImage(
        [&](FILE* fp){
            fwrite(&bmpFileInfo,sizeof bmpFileInfo,1,fp);
            fwrite(&bmpInfo,sizeof bmpInfo,1,fp);
            fwrite(&bmfColorQuad,sizeof bmfColorQuad,1,fp);
            fwrite(pData,w*h,1,fp);
        },0
    );
}

bool saveImage(int w,int h,uint8_t* pData,MyString fileName){
    if(test.isTest(test.UI)){
        return true;
    }
    ImageInit(w,h);
    return MyFile::SaveImage(
        fileName,[&](FILE* fp){
            fwrite(&bmpFileInfo,sizeof bmpFileInfo,1,fp);
            fwrite(&bmpInfo,sizeof bmpInfo,1,fp);
            fwrite(&bmfColorQuad,sizeof bmfColorQuad,1,fp);
            fwrite(pData,w*h,1,fp);
        }
    );
}

bool saveBGImg(int w,int h,uint8_t* pData,MyString fileName){
    if(test.isTest(test.UI)){
        return true;
    }
    ImageInit(w,h);
    return MyFile::SaveBGImg(
        fileName,[&](FILE* fp){
            fwrite(&bmpFileInfo,sizeof bmpFileInfo,1,fp);
            fwrite(&bmpInfo,sizeof bmpInfo,1,fp);
            fwrite(&bmfColorQuad,sizeof bmfColorQuad,1,fp);
            fwrite(pData,w*h,1,fp);
        }
    );
}

// 分析数据包里的数据,然后用save函数保存图像
void analysis(DataPacket dataPacket,std::function<void(int w,int h,uint8_t* pData,MyString fileName)> save){
    MyString fileName=MyString::Time()+".bmp";

    int w,h,dataSize=dataPacket.readSize();
    uint8_t* pData=nullptr;
    uint8_t* x2=nullptr;
    switch(dataSize){
    case 160*160/2:
        w=h=160;
        //把图像从4bit转化为8bit
        pData=dataPacket.getPointer();
        x2=new uint8_t[160*160+5];
        for(int i=0;i<160;i++){
            for(int j=0;j<80;j++){
                x2[i*160+j*2]=pData[i*80+j]&0xF0;
                x2[i*160+j*2+1]=(pData[i*80+j]&0x0F)<<4;
            }
        }
        pData=x2;
    case 160*160:
        MyLog::user("接收到160x160的图像");
        w=h=160;
        // 为了提高代码复用度,如果pData为空指针说明是直接跳到160*160的
        // 如果pData不为空,说明是从上一个case下来的
        if(!pData){
            pData=dataPacket.getPointer();
        }
        //消除用于静电检测的竖线
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                if(j==47||j==99||j==151){
                    pData[i*w+j]=(pData[i*w+j+1]+pData[i*w+j-1])/2;
                }
            }
        }
        reverse(pData,w*h);
        for(int i=0;i<h;i++){
            reverse(pData+i*w,w);
        }
        save(w,h,pData,fileName);
        break;
    case 192*192:
        MyLog::user("接收到192x192的图像");
        w=h=192;

        save(w,h,dataPacket.getPointer(),fileName);
        break;
    default:
        MyLog::user("既不是160x160也不是192x192,没法渲染图像");
        break;
    }
    if(x2){
        delete[] x2;
    }
    dataPacket.readData(dataPacket.size());
}

// 把dataPacket里的数据当做图片存到dir路径下
void saveImage(DataPacket dataPacket){
    analysis(dataPacket,
        [&](int w,int h,uint8_t* pData,MyString fileName){
            saveImage(w,h,pData,fileName);
            loadImage(image,MyFile::IMAGE_DIR+fileName);
        }
    );
}

void saveBGImg(DataPacket dataPacket){
    analysis(dataPacket,
        [&](int w,int h,uint8_t* pData,MyString fileName){
            saveBGImg(w,h,pData,fileName);
            loadImage(image,MyFile::BACKGROUND_DIR+fileName);
        }
    );
}

// 生成亮度直方图,输入为w*h的pData,输出为hw*hh的Histogram
void generateHistogram(uint8_t* Histogram,int hw,int hh,uint8_t* pData,int w,int h){
    auto func=[&](int position,int height)->void{
        for(int i=0;i<hh;i++){
            if(i==height){
                if(position>0){
                    Histogram[i*hw+position-1]=127;
                }
                if(position<hw){
                    Histogram[i*hw+position+1]=127;
                }
                Histogram[i*hw+position]=0;
            } else if(i==height+1||i==height-1){
                Histogram[i*hw+position]=127;
            } else{
                Histogram[i*hw+position]=255;
            }
        }
    };
    int* count=new int[hw];
    memset(count,0,hw*4);
    for(int i=0;i<w*h;i++){
        count[pData[i]*hw/256]++;
    }
    int max=0;
    for(int i=0;i<hw;i++){
        if(max<count[i]){
            max=count[i];
        }
    }
    for(int i=0;i<hw;i++){
        func(i,count[i]*hh/max);
    }
    delete[] count;
}

// 把w*h的src的图片放大一倍,存到dest里
void imgSizeX2(uint8_t * src,int w,int h,uint8_t * dest){
    int dw=w+w,dh=h+h;
    for(int i=0;i<dh;i++){
        for(int j=0;j<dw;j++){
            dest[i*dw+j]=src[i/2*w+j/2];
        }
    }
}
