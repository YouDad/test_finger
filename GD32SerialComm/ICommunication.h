#pragma once

class ICommunication{
    public:
        ICommunication& Instance();
        bool Connect(int id,int baud);
        bool DisConnect();
        bool IsOpen();
        void Request(int cmdCode,DataPacket packet=DataPacket());
    protected:
        int id;
        MyThread* response;
        MyThread* listener;
};
