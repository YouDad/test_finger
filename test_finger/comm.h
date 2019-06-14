#pragma once
#include"stdafx.h"

// 通信设备 Communication Device
class Comm{
public:
    Comm();
    // 获得ID
    int getConnectId();
    // 连接下位机
    bool connect(int id=0,int baud=0);
    // 断开与下位机的连接
    bool disconnect();
    // 向下位机发送命令(数据)
    void request(int cmdCode,DataPacket packet=DataPacket());
    // 设置request的阻塞方式,阻塞的话,收到回应之后再继续发
    void setBlock(bool block);
private:
    // 串口数据成员
    CSerial serial;
    // USB数据成员
    CUsbPort usb;
    // 0:USB,>0:Serial,<0:None
    int id;
    // 是否位于阻塞态
    bool block;
    // 在阻塞态下保证只有一个命令被发出去
    MyLocker blockLocker;

    // 负责分发Packet给对应的Listener,是Packet的消费者
    MyThread* responseThread;
    // 负责监听下位机的数据,是Packet的生产者
    MyThread* listenThread;
    // 向设备发送字节流
    void sendBytes(uint8_t* data,int len);
    // 从设备中读取字节流,返回是否读到内容
    bool readBytes(uint8_t* buffer,int blen,ULONG& cnt);
    // 返回设备是否打开
    bool isOpen();
};

// 全局唯一通信设备
extern Comm comm;
// 给UsbPort.cpp用的,-1代表未连接
extern int CommType;
