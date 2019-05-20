#include "stdafx.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainDialog::MainDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(MainDialog::IDD,pParent){
    m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(MainDialog,CDialogEx)
    ON_WM_PAINT()
    ON_MESSAGE(WM_GET_RAW_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_IMAGE,serialResponse)
    ON_MESSAGE(WM_STP_GET_IMAGE,serialResponse)
    ON_MESSAGE(WM_READ_REGISTER,serialResponse)
    ON_MESSAGE(WM_APPEND_CONTROLS,serialResponse)
    ON_MESSAGE(WM_GET_TEST_IMAGE,serialResponse)
    ON_MESSAGE(WM_GET_CON_BKI,serialResponse)
    ON_MESSAGE(WM_STP_GET_BKI,serialResponse)
    ON_WM_QUERYDRAGICON()
    ON_WM_DEVICECHANGE()
    ON_BN_CLICKED(IDC_BTNConnect,&MainDialog::OnBnClickedBtnconnect)
    ON_BN_CLICKED(IDC_BTNSaveLog,&MainDialog::OnBnClickedBtnsavelog)
    ON_BN_CLICKED(IDC_BTNRawImage,&MainDialog::OnBnClickedBtnrawimage)
    ON_BN_CLICKED(IDC_BTNContinueImage,&MainDialog::OnBnClickedBtncontinueimage)
    ON_BN_CLICKED(IDC_BTNdevLog,&MainDialog::OnBnClickedBtndevlog)
    ON_BN_CLICKED(IDC_BTNreadReg,&MainDialog::OnBnClickedBtnreadreg)
    ON_BN_CLICKED(IDC_BTNwriteReg,&MainDialog::OnBnClickedBtnwritereg)
    ON_BN_CLICKED(IDC_BTNOpenImage,&MainDialog::OnBnClickedBtnopenimage)
    ON_BN_CLICKED(IDC_BTNContinueBackGroundImage,&MainDialog::OnBnClickedBtncontinuebackgroundimage)
    ON_BN_CLICKED(IDC_BTNOpenBackGroundImage,&MainDialog::OnBnClickedBtnopenbackgroundimage)
    ON_BN_CLICKED(IDC_BTNTestImage,&MainDialog::OnBnClickedBtnbackgroundimage)
    ON_CBN_CLOSEUP(IDC_CMBLogLevel,&MainDialog::OnCbnCloseupCmbloglevel)
    ON_BN_CLICKED(IDC_BTNAdvDbg,&MainDialog::OnBnClickedBtnadvdbg)
    ON_BN_CLICKED(IDC_BTN0,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN1,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN2,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN3,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN4,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN5,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN6,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTN7,&MainDialog::OnBnClickedBtn)
    ON_BN_CLICKED(IDC_BTNEnroll,&MainDialog::OnBnClickedBtnenroll)
    ON_BN_CLICKED(IDC_BTNMatch,&MainDialog::OnBnClickedBtnmatch)
    ON_BN_CLICKED(IDC_BTNDeviceInfo,&MainDialog::OnBnClickedBtndeviceinfo)
    ON_BN_CLICKED(IDC_BTNViewEnrollIds,&MainDialog::OnBnClickedBtnviewenrollids)
    ON_BN_CLICKED(IDC_BTNDeleteTemplate,&MainDialog::OnBnClickedBtndeletetemplate)
    ON_BN_CLICKED(IDC_BTNCancel,&MainDialog::OnBnClickedBtncancel)
    ON_BN_CLICKED(IDC_BTNClearLog,&MainDialog::OnBnClickedBtnclearlog)
END_MESSAGE_MAP()

void MainDialog::OnOK(){}

BOOL MainDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    SetIcon(m_hIcon,TRUE);
    SetIcon(m_hIcon,FALSE);

    ///1.全局控件赋值
    initMyControl(this);

    ///2.对组合框的初始化
    updateCommunityWay();

    ///3.更新 各控件访问权限
    CtrlValidity::InitCtrl();
    CtrlValidity::Init();

    return TRUE;//除非将焦点设置到控件，否则返回 TRUE
}

void MainDialog::OnPaint(){
    if(IsIconic()){
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND,reinterpret_cast<WPARAM>(dc.GetSafeHdc()),0);
        int cxIcon=GetSystemMetrics(SM_CXICON);
        int cyIcon=GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x=(rect.Width()-cxIcon+1)/2;
        int y=(rect.Height()-cyIcon+1)/2;
        dc.DrawIcon(x,y,m_hIcon);
    } else{
        CDialogEx::OnPaint();
    }
}

HCURSOR MainDialog::OnQueryDragIcon(){
    return static_cast<HCURSOR>(m_hIcon);
}

//支持串口热拔插的设备更改监听函数
BOOL MainDialog::OnDeviceChange(UINT nEventType,DWORD dwData){
    switch(nEventType){
        case DBT_DEVICEREMOVECOMPLETE://移除设备
            updateCommunityWay();
            autoDisconnect();
            break;
        case DBT_DEVICEARRIVAL://添加设备
            updateCommunityWay();
            autoConnect();
            break;
    }
    return TRUE;
}

//连接下位机按钮的点击事件
void MainDialog::OnBnClickedBtnconnect(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnConnect)=="连接下位机"){
        CtrlValidity::Connecting();

        int com,baud=getInt(cmbBaud);//读取波特率
        sscanf(getText(cmbWay),"COM%d",&com);//读取通信方式

        bool ret=comm.connect(com,baud);
        if(ret){
            setText(btnConnect,"断开连接");
            CtrlValidity::AfterConnect();
        } else{
            CtrlValidity::BeforeConnect();
        }
    } else{
        comm.disconnect();
        CtrlValidity::BeforeConnect();
        setText(btnConnect,"连接下位机");
    }
}

//日志保存按钮点击事件
void MainDialog::OnBnClickedBtnsavelog(){
    //获取文件路径名
    LPCTSTR filter=_T("文本文件(*.txt)|*.txt||");
    CFileDialog dlgFileOpen(0,0,0,0,filter,0);
    if(dlgFileOpen.DoModal()==IDOK){
        MyString path=dlgFileOpen.GetPathName();
        //Add .txt
        if(path.find(".txt",path.length()-4)==-1){
            path=path+".txt";
        }
        //write into file
        FILE* fp=fopen(path,"w");
        fprintf_s(fp,"%s",(const char*)getText(editLog));
        fclose(fp);
    }
}

HANDLE timeoutThread_continueImage_Mutex=CreateMutex(0,0,0);
bool timeoutThread_continueImage;

MyThread ImageTimeout(ThreadFunction__(timeoutFunction)(void){
    Sleep(10*1000);
    WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
    if(!timeoutThread_continueImage){
        MyLog.print(Log::LOGU,"采图超时");
        CtrlValidity::Work();
    }
    ReleaseMutex(timeoutThread_continueImage_Mutex);
});


MyThread RegisterTimeout(ThreadFunction__(timeoutFunction)(void){
    Sleep(1*1000);
    MyLog.print(Log::LOGU,"读寄存器超时");
    CtrlValidity::Work();
});

//原始图像的点击事件
void MainDialog::OnBnClickedBtnrawimage(){
    CtrlValidity::Working();
    comm.request(SII(GetRawImage));
    progress->SetPos(30);
    MyLog.print(Log::LOGU,"请放手指");
    ImageTimeout.start();
}

//串口线程消息处理函数
LRESULT MainDialog::serialResponse(WPARAM w,LPARAM l){
    static bool continueImage=false;
    switch(w){
        case WM_GET_CON_IMAGE:
        {
            continueImage=true;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=true;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
        }
        case WM_GET_RAW_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(SII(GetRawImage));
                progress->SetPos(30);
                MyLog.print(Log::LOGU,"请放手指");
                ImageTimeout.start();
            } else{
                CtrlValidity::Work();
            }
        }break;
        case WM_STP_GET_IMAGE:
        {
            continueImage=false;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=false;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
            CtrlValidity::Work();
            ImageTimeout.terminate();
            progress->SetPos(0);
        }break;
        case WM_READ_REGISTER:
        {
            RegisterTimeout.terminate();
            progress->SetPos(100);
        }break;
        case WM_APPEND_CONTROLS:
        {
            advancedDebugDialog->append(MyString::ParseInt($::conf["id"]),$::conf["AdvDbg_ImgId"]);
            $::conf["AdvDbg_ImgId"]=MyString::IntToMyString(MyString::ParseInt($::conf["AdvDbg_ImgId"])+1);
        }break;
        case WM_GET_CON_BKI:
        {
            continueImage=true;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=true;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
        }
        case WM_GET_TEST_IMAGE:
        {
            ImageTimeout.terminate();
            if(continueImage){
                comm.request(SII(GetTestImage));
                progress->SetPos(30);
                ImageTimeout.start();
            } else{
                CtrlValidity::Work();
            }
        }break;
        case WM_STP_GET_BKI:
        {
            continueImage=false;
            WaitForSingleObject(timeoutThread_continueImage_Mutex,-1);
            timeoutThread_continueImage=false;
            ReleaseMutex(timeoutThread_continueImage_Mutex);
            CtrlValidity::Work();
            ImageTimeout.terminate();
            progress->SetPos(0);
        }break;
    }
    progress->SetPos(0);
    return 1;
}

//连续获取图像的点击事件
void MainDialog::OnBnClickedBtncontinueimage(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueImage)=="连续获取图像"){
        MyLog.print(Log::LOGU,"开始连续获取图像");
        CtrlValidity::Working(CtrlValidity::vec{btnContinueImage});
        setText(btnContinueImage,"停止获取图像");
        SendMessage(WM_GET_CON_IMAGE,WM_GET_CON_IMAGE,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取图像");
        CtrlValidity::Work();
        setText(btnContinueImage,"连续获取图像");
        SendMessage(WM_STP_GET_IMAGE,WM_STP_GET_IMAGE,0);
    }
}


void MainDialog::OnBnClickedBtndevlog(){
    MyLog.DevelopLog();
}

void MainDialog::OnBnClickedBtnreadreg(){
    CtrlValidity::Working();
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始读寄存器");

    uint8_t address=getHex(editReadRegAddr);

    comm.request(SII(ReadRegister),&address,1);

    progress->SetPos(60);
    RegisterTimeout.start();
}


void MainDialog::OnBnClickedBtnwritereg(){
    CtrlValidity::Working();
    progress->SetPos(50);
    MyLog.print(Log::LOGD,"开始写寄存器");

    uint8_t addrVal[2];
    addrVal[0]=getHex(editWriteRegAddr);
    addrVal[1]=getHex(editWriteRegVal);

    comm.request(SII(WriteRegister),addrVal,2);

    progress->SetPos(100);
    CtrlValidity::Work();
}


void MainDialog::OnBnClickedBtncontinuebackgroundimage(){
    //根据按钮上的文字判断当前连接状态
    if(getText(btnContinueBackGroundImage)=="连续获取背景"){
        MyLog.print(Log::LOGU,"开始连续获取背景");
        CtrlValidity::Working(CtrlValidity::vec{btnContinueBackGroundImage});
        setText(btnContinueBackGroundImage,"停止获取背景");
        SendMessage(WM_GET_CON_BKI,WM_GET_CON_BKI,0);
    } else{
        MyLog.print(Log::LOGU,"停止连续获取背景");
        CtrlValidity::Work();
        setText(btnContinueBackGroundImage,"连续获取背景");
        SendMessage(WM_STP_GET_BKI,WM_STP_GET_BKI,0);
    }
}


void MainDialog::OnBnClickedBtnopenimage(){
    if(access("collectedImage",0)){
        MyLog.print(Log::LOGU,"图片文件夹不存在,请先采一张图片");
    } else{
        ShellExecuteA(NULL,"explore","collectedImage",NULL,NULL,SW_NORMAL);
    }
}


void MainDialog::OnBnClickedBtnopenbackgroundimage(){
    if(access("collectedBGI",0)){
        MyLog.print(Log::LOGU,"背景文件夹不存在,请先采一张背景");
    } else{
        ShellExecuteA(NULL,"explore","collectedBGI",NULL,NULL,SW_NORMAL);
    }
}


void MainDialog::OnBnClickedBtnbackgroundimage(){
    CtrlValidity::Working();
    progress->SetPos(30);
    MyLog.print(Log::LOGD,"开始采集背景");
    comm.request(SII(GetTestImage));
    progress->SetPos(60);
}


void MainDialog::OnCbnCloseupCmbloglevel(){
    static clock_t firstTime=0,secondTime=0;
    if(firstTime==0){
        firstTime=clock();
    } else{
        if(secondTime==0){
            secondTime=clock();
        } else{
            clock_t now=clock();
            if(now-firstTime>2000){
                firstTime=secondTime;
                secondTime=now;
            } else{
                $::conf["AdvDbg"]="true";
                btnAdvDbg->ShowWindow(SW_SHOW);
            }
        }
    }
}


void MainDialog::OnBnClickedBtnadvdbg(){
    if(advancedDebugDialog){
        delete advancedDebugDialog;
        advancedDebugDialog=0;
    } else{
        advancedDebugDialog=new AdvancedDebugDialog();
        advancedDebugDialog->Create(IDD_ADVANCED_DEBUG_DIALOG,this);
        advancedDebugDialog->ShowWindow(SW_SHOW);
    }
}


void MainDialog::OnBnClickedBtn(){
    setText(editFingerId,getText(GetFocus()));
}


void MainDialog::OnBnClickedBtnenroll(){
    if(getText(editFingerId)==""){
        MyLog.user("编号不能为空");
        return;
    }
    static uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
    static uint8_t BufferID;
    BufferID=1;
    static const int EnrollCount=3;
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        CtrlValidity::Working();
        comm.request(SII(GetEnrollImage));//获取图像
        FlowID++;
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        if(result==0x00){//取到图像
            MyLog.debug("取到指纹图像(%d/%d)",BufferID,EnrollCount);
            comm.request(SII(UpImage));//上传图像
            FlowID++;
            return false;
        } else{//没取到图像
            FlowID--;
            return true;
        }
    });
    Flow.push_back(FlowFunction(2)(int& result){
        if(result==0x00){//上传图像成功
            comm.request(SII(GenChar),&BufferID,1);//生成特征
            FlowID++;
            return false;
        } else{//上传图像失败
            MyLog.debug("上传图像失败");
            FlowID=12;
            return true;
        }
    });
    Flow.push_back(FlowFunction(3)(int& result){
        if(result==0x00){//生成特征成功
            FlowID++;
            MyLog.debug("生成特征成功(%d/%d)",BufferID,EnrollCount);

            uint8_t x[]={BufferID,0,0,0,0x87};
            comm.request(SII(Search),x,sizeof x);//搜索指纹
            FlowID++;
            return false;
        } else{//生成特征失败
            FlowID=0;
            return true;
        }
    });
    Flow.push_back(FlowFunction(4)(int& result){
        if(result==0x09){//没搜到指纹
            MyLog.debug("等待手指移开");
            comm.request(SII(GetRawImage));//等待把手指移开
            FlowID++;
            return false;
        } else{//搜到指纹
            MyLog.debug("已有指纹");
            FlowID=12;
            return true;
        }
    });
    Flow.push_back(FlowFunction(5)(int& result){
        if(result==0x02){//没指纹
            FlowID++;
        } else{//有指纹
            result=0x09;
            FlowID--;
        }
        return true;
    });
    Flow.push_back(FlowFunction(6)(int& result){
        if(BufferID>=EnrollCount){//达到录入次数
            FlowID++;
        } else{//没达到录入次数
            BufferID++;
            FlowID=0;
        }
        return true;
    });
    Flow.push_back(FlowFunction(7)(int& result){
        comm.request(SII(RegModel));//合并特征
        FlowID++;
        return false;
    });
    Flow.push_back(FlowFunction(8)(int& result){
        if(result==0x00){//合并特征成功
            FlowID++;
            MyLog.debug("合并特征成功");
        } else{//合并失败
            FlowID=12;
        }
        return true;
    });
    Flow.push_back(FlowFunction(9)(int& result){
        uint8_t data[]={1,0,FingerID};
        comm.request(SII(StoreChar),data,sizeof data);//存储模板
        FlowID++;
        return false;
    });
    Flow.push_back(FlowFunction(10)(int& result){
        if(result==0x00){//存储模板成功
            FlowID++;
            MyLog.debug("存储模板成功");
        } else{//存储失败
            FlowID=12;
        }
        return true;
    });
    Flow.push_back(FlowFunction(11)(int& result){//注册成功
        FlowID=0;
        BufferID=1;
        MyLog.print(Log::LOGU,"注册成功");
        Flow.clear();
        CtrlValidity::Work();
        return false;
    });
    Flow.push_back(FlowFunction(12)(int& result){//注册失败
        CtrlValidity::Work();
        FlowID=0;
        BufferID=1;
        MyLog.print(Log::LOGU,"注册失败");
        Flow.clear();
        return false;
    });
    int tmp=0;
    ExecFlow(tmp);
}


void MainDialog::OnBnClickedBtnmatch(){
    if(getText(editFingerId)==""){
        MyLog.user("编号不能为空");
        return;
    }
    static uint8_t FingerID=MyString::ParseInt(getText(editFingerId));
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        CtrlValidity::Working();
        uint8_t data[]={02,00,FingerID};
        comm.request(SII(LoadChar),data,sizeof data);
        FlowID++;
        MyLog.user("读出指纹模板中...");
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        if(result==0x00){
            comm.request(SII(GetRawImage));
            FlowID++;
            return false;
        } else{
            FlowID=5;
            return true;
        }
    });
    Flow.push_back(FlowFunction(2)(int& result){
        if(result==0x00){
            MyLog.user("取指纹图成功");
            uint8_t data[]={02};
            comm.request(SII(GenChar),data,sizeof data);
            FlowID++;
            return false;
        } else{
            result=0x00;
            FlowID--;
            return true;
        }
    });
    Flow.push_back(FlowFunction(3)(int& result){
        if(result==0x00){
            MyLog.user("指纹生成特征成功");
            comm.request(SII(Match));
            FlowID++;
            return false;
        } else{
            MyLog.user("指纹生成特征失败");
            result=0x00;
            FlowID--;//回退两步
            FlowID--;//回退两步
            return true;
        }
    });
    Flow.push_back(FlowFunction(4)(int& result){
        if(result==0x00){
            MyLog.user("指纹匹配成功");
            FlowID++;
            return true;
        } else{
            MyLog.user("指纹不匹配");
            FlowID++;
            return true;
        }
    });
    Flow.push_back(FlowFunction(5)(int& result){
        CtrlValidity::Work();
        MyLog.user("比对结束");
        FlowID=0;
        Flow.clear();
        return false;
    });
    int tmp=0;
    ExecFlow(tmp);
}


void MainDialog::OnBnClickedBtndeviceinfo(){
    //TODO 设备信息
}


void MainDialog::OnBnClickedBtnviewenrollids(){
    Flow.clear();
    Flow.push_back(FlowFunction(0)(int& result){
        CtrlValidity::Working();
        uint8_t ReadIndexTable[]={0};
        comm.request(SII(ReadIndexTable),ReadIndexTable,1);
        FlowID++;
        return false;
    });
    Flow.push_back(FlowFunction(1)(int& result){
        CtrlValidity::Work();
        FlowID=0;
        Flow.clear();
        return false;
    });
    int tmp=0;
    ExecFlow(tmp);
}


void MainDialog::OnBnClickedBtndeletetemplate(){
    //TODO 删除按钮
}


void MainDialog::OnBnClickedBtncancel(){
    int tmp=Flow.size()-1;
    ExecFlow(tmp);
    MyLog.user("取消了操作");
    CtrlValidity::Work();
    if(lastCmdCode.size()){
        lastCmdCode.pop();
    }
}


void MainDialog::OnBnClickedBtnclearlog(){
    MyLog.ClearLog();
}
