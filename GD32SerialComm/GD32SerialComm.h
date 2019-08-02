class GD32SC: class IComm{
    public:
        IComm& Instance();
        bool Connect(int id,int baud);
        bool DisConnect();
        bool IsOpen();
        void Request(int cmdCode,DataPacket packet=DataPacket());
    private:
        CSerial serial;
        int id;
        MyThread* response;
        MyThread* listener;
};
