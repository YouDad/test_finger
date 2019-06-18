#include "stdafx.h"

std::vector<MyThread*> blockQueue;
int CommType;
Comm comm;

Comm::Comm(){
    this->block=false;
    this->id=CommType=-1;

    // 这个缓冲区基本上会一直用,而且是单线程使用,所以是static的
    static uint8_t buffer[1<<16];

    // 访问dataQueue(Packet缓冲区)的锁
    static MyLocker dataQueueMutex;

    // Packet缓冲区
    static std::queue<DataPacket> dataQueue;

    // 负责监听下位机的数据,是Packet的生产者
    this->listenThread=new MyThread(
        [&](){
            while(this->isOpen()){
                ULONG cnt;
                if(this->readBytes(buffer,sizeof buffer,cnt)){
                    cnt=max(cnt,sizeof buffer);
                    dataQueue.push(DataPacket(buffer,cnt));
                    dataQueueMutex.unlock();
                }
            }
            return 0;
        }
    );

    // 负责分发Packet给对应的Listener,是Packet的消费者
    this->responseThread=new MyThread(
        [&](){
            DataPacket dataPacket;
            while(1){
                // 堵在访问缓冲区的锁里
                dataQueueMutex.lock();

                // 能通过代表缓冲区已经有数据了
                if(dataQueue.size()){
                    dataPacket=dataQueue.front();
                    dataQueue.pop();
                }
                while(dataPacket.isValid()){
                    // 获得这个数据包包含的指令
                    int cmdCode=converterBoardcast.ResponseGetCmdCode(dataPacket);

                    // cmdCode==-1代表没有协议可以转化剩下的数据了,数据废了
                    if(cmdCode==-1){
                        break;
                    }

                    // 获得这个数据包包含的数据
                    DataPacket data=converterBoardcast.ResponseConvert(dataPacket);

                    // 按照指令和数据进行广播
                    listenerBoardcast.boardcast(cmdCode,data);
                    data.Destruction();
                }
                dataPacket.Destruction();
                if(this->block){
                    this->blockLocker.unlock();
                }
            }
        }
    );
}

// 获得ID
int Comm::getConnectId(){
    return this->id;
}

// 设置request的阻塞方式,阻塞的话,收到回应之后再继续发
void Comm::setBlock(bool block){
    this->block=block;
}

// 向下位机发送命令(数据)
void Comm::request(int cmdCode,DataPacket packet){
    blockQueue.push_back(new MyThread(
        [=](){
            // 把cmdCode和packet按照所选协议来转化为对应格式
            auto converter=converterBoardcast.RequestConvert();
            if(converter){
                // 转化为输出用的Packet
                auto dataPacket=converter->convert(cmdCode,packet.getPointer(),packet.size());
                for(auto it=dataPacket.begin();it!=dataPacket.end();it++){
                    this->sendBytes(it->getPointer(),it->size());
                    it->Destruction();
                }
            } else{
                // 找不到Converter?
                ASF_ERROR(3);
            }
        },true
    ));
    (new MyThread(
        [=](){
            if(this->block){
                this->blockLocker.lock();
            }
            blockQueue.back()->start();
            blockQueue.pop_back();
        },true
    ))->start();
}

// 返回设备是否打开
bool Comm::isOpen(){
    if(test.isTest(test.Comm)){
        return true;
    }
    if(this->id>0){
        return this->serial.IsOpen();
    } else{
        return this->id==0;
    }
}

// 向设备发送字节流
void Comm::sendBytes(uint8_t * data,int len){
    if(!this->isOpen()){
        return;
    }
    if(test.isTest(test.Comm)){
        memcpy(test.commTest.Comm_send,data,len);
        test.commTest.Comm_sendSize=len;
        return;
    }
    if(this->id==0){
        BYTE cdb_w[16]={0xef,0x01};
        this->usb.USBSCSIWrite(cdb_w,IOCTRL_CDB_LEN,data,len,SCSI_TIMEOUT);
    } else{
        this->serial.Write(data,len);
    }
}

// 从设备中读取字节流,返回是否读到内容
bool Comm::readBytes(uint8_t* buffer,int blen,ULONG& cnt){
    if(!this->isOpen()){
        return false;
    }
    if(test.isTest(test.Comm)){
        if(test.commTest.Comm_receiveSize>0){
            cnt=test.commTest.Comm_receiveSize;
            memcpy(buffer,test.commTest.Comm_receive,cnt);
            test.commTest.Comm_receiveSize=0;
            return true;
        }
        return false;
    }
    if(this->id==0){
        BYTE cdb_r[16]={0xef,0x02};
        return this->usb.USBSCSIRead(cdb_r,IOCTRL_CDB_LEN,buffer,blen,cnt,SCSI_TIMEOUT);
    } else{
        return this->serial.Read(buffer,blen,&cnt)==ERROR_SUCCESS;
    }
}

// 连接下位机
bool Comm::connect(int id,int baud){
    if(test.isTest(test.Comm)){
        this->responseThread->start();
        this->listenThread->start();
        return true;
    }
    // USB方式连接
    if(id==0){
        int ret=this->usb.InitUsbPort(COMM_USB_MASS,"UD");
        if(ret!=-1){
            MyLog::user("连接USB成功");
            this->id=CommType=COMM_USB_MASS;
            this->responseThread->start();
            this->listenThread->start();
        } else{
            MyLog::user("连接USB失败(注意,连接USB需要管理员权限)");
            this->id=CommType=-1;
        }
        return ret!=-1;
    }
    // 串口方式连接

    // 构造端口地址
    MyString way=MyString::Format("\\\\.\\COM%d",id);
    LONG retCode=ERROR_SUCCESS;
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.Open(way,2048,2048,true); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.Setup((CSerial::EBaudrate)baud); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.SetMask(CSerial::EEventRecv); }
    if(retCode==ERROR_SUCCESS){ retCode=this->serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking); }
    if(retCode==ERROR_SUCCESS){ this->serial.Purge(); }
    if(retCode==ERROR_SUCCESS){
        this->responseThread->start();
        this->listenThread->start();
        this->id=id;
        MyLog::user("连接COM%d成功",id);
    } else{
        MyLog::user("连接COM%d失败",id);
    }
    return retCode==ERROR_SUCCESS;
}

// 断开与下位机的连接
bool Comm::disconnect(){
    if(test.isTest(test.Comm)){
        this->responseThread->terminate();
        this->listenThread->terminate();
        return true;
    }
    if(this->id<0){
        return true;
    }
    this->responseThread->terminate();
    this->listenThread->terminate();
    // 根据id来判断是串口还是USB
    int ret=ERROR_SUCCESS;
    if(this->id==0){
        ret=this->usb.CloseUsbPort();
    } else{
        ret=this->serial.Close();
    }
    MyLog::user("断开连接成功");
    if(ret==ERROR_SUCCESS){
        this->id=CommType=-1;
        return true;
    } else{
        ASF_WARNING(5);
        return false;
    }
}
