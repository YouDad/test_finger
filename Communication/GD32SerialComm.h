#pragma once
#include "ICommunication.h"

namespace Communication{
    using ICommunication::ICommunication;
    namespace GD32SerialComm{
        class GD32SC: public ICommunication{
        public:
            ICommunication& Instance();
            bool Connect(int id,int baud);
            bool DisConnect();
            bool IsOpen();
            //void Request(int cmdCode,DataPacket packet=DataPacket());
        private:
            //CSerial serial;
            int id;
            //MyThread* response;
            //MyThread* listener;
        };
    }
}
