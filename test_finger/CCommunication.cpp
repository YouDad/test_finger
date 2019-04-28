#include"stdafx.h"
//CSerial CCommunication::serial;
//int CCommunication::id=0;
//int CommType;
//
//bool isUSB(){
//    CString selectionText;
//    cmbWay->GetWindowText(selectionText);
//    return selectionText.Compare(_T("USB"))==0;
//}
//
//int CCommunication::getConnectId(){
//    return CCommunication::id;
//}
//
//bool CCommunication::connect(int id,int baud){
//    if(isUSB()){
//        if(UsbPort.InitUsbPort(COMM_USB_MASS,"UD")){
//            CommType=COMM_USB_MASS;
//            cmbChipType->SetCurSel(1);
//            bool ret=sendCommand(USR_CMD_GET_INFO);
//            ret=waitForPacket(100*1000);
//            return true;
//        } else{
//            CommType=-1;
//            return false;
//        }
//    }
//    CString way;
//    way.Format(_T("\\\\.\\COM%d"),id);//得到端口地址
//    LONG retCode=ERROR_SUCCESS;
//
//    if(retCode==ERROR_SUCCESS){
//        retCode=serial.Open(way,2048,2048,true);
//    }
//    //其余三参数均采用默认参数
//    if(retCode==ERROR_SUCCESS){
//        retCode=serial.Setup((CSerial::EBaudrate)baud);
//    }
//    if(retCode==ERROR_SUCCESS){
//        retCode=serial.SetMask(CSerial::EEventRecv);
//    }
//    if(retCode==ERROR_SUCCESS){
//        serial.Purge();
//    }
//    MyLog.print(Log::LOGU,"连接COM%d%s",id,retCode==ERROR_SUCCESS?"成功":"失败");
//    if(retCode==ERROR_SUCCESS){
//        CCommunication::id=id;
//    }
//    return retCode==ERROR_SUCCESS;
//}
//
//bool CCommunication::disConnect(){
//    if(isUSB()){
//        return UsbPort.CloseUsbPort()==0;
//    }
//    int ret=serial.Close();
//    MyLog.print(Log::LOGU,"断开连接成功");
//    if(ret==ERROR_SUCCESS){
//        CCommunication::id=0;
//        return true;
//    } else{
//        ASF_WARNING(5);
//        return false;
//    }
//}
//
//bool CCommunication::sendCommand(int CmdCode,uint8_t* Data,uint16_t Len){
//    switch(cmbChipType->GetCurSel()){
//        case 0:return sendCommand_Default(CmdCode,Data,Len);
//        case 1:
//            switch(CmdCode){
//                case USR_CMD_GET_INFO:
//                    return sendCommand_HangXin(USR_CMD_GET_INFO);
//                    break;/*航芯:返回芯片信息*/
//                case CMD_GET_TEST_IMAGE:      break;/*获得背景图片*/
//                case CMD_DEVICE_RESET:        break;/*系统复位*/
//                case CMD_DETECT_FINGER:       break;/*探测指纹*/
//                case CMD_GET_RAW_IMAGE:
//                    sendCommand_HangXin(USR_CMD_GRAB_NO_CHECK);
//                    waitForPacket(10*1000);
//                    return sendCommand_HangXin(USR_CMD_UP_IMG);
//                    break;/*采集原始图像*/
//                case CMD_GET_REDRESS_IMAGE:   break;/*采集矫正图像*/
//                case CMD_UPLOAD_IMAGE:        break;/*上传图像(包括原始图像和矫正图像)*/
//                case CMD_GEN_CHAR:            break;/*生成模板*/
//                case CMD_MATCH_CHAR:          break;/*比对模板*/
//                case CMD_STORE_CHAR:          break;/*存储模板*/
//                case CMD_SEARCH_CHAR:         break;/*搜索模板*/
//                case CMD_DELETE_CHAR:         break;/*删除指定模板*/
//                case CMD_EMPTY_CHAR:          break;/*清空模板库*/
//                case CMD_VERIFY_CHAR:         break;/*单一比对模板*/
//                case CMD_GET_CHAR:            break;/*获取模板*/
//                case CMD_PUT_CHAR:            break;/*下载模板*/
//                case CMD_GET_MBINDEX:         break;/*获取MBIndex*/
//                case CMD_READ_NOTE_BOOK:      break;/*读记事本*/
//                case CMD_WRITE_NOTE_BOOK:     break;/*写记事本*/
//                case CMD_READ_PAR_TABLE:      break;/*读取参数表*/
//                case CMD_SET_BAUD_RATE:       break;/*设置波特率*/
//                case CMD_SET_SECURLEVEL:      break;/*设置指纹安全级别*/
//                case CMD_SET_CMOS_PARA:       break;/*设置CMOS参数*/
//                case CMD_RESUME_FACTORY:      break;/*恢复出厂设置*/
//                case CMD_MERGE_CHAR:          break;/*合并特征*/
//                case CMD_SET_PSW:             break;/*密码设置*/
//                case CMD_SET_ADDRESS:         break;/*地址设置*/
//                case CMD_GET_RANDOM:          break;/*获取随机数*/
//                case CMD_DOWNLOAD_IMAGE:      break;/*下载指纹图像*/
//                case CMD_ERASE_PROGRAM:       break;/*擦除程序标识*/
//                case CMD_STORE_CHAR_DIRECT:   break;/*直接存储指纹模板*/
//                case CMD_READ_CHAR_DIRECT:    break;/*直接根据地址读取指纹模板*/
//                case CMD_GET_FIRSTVALID_ADD:  break;/*获取第一个空闲的指纹索引*/
//                case CMD_CHIP_ERASE:          break;/*整片擦除FLASH*/
//            }
//            return sendCommand_HangXin(CmdCode,Data,Len);
//        default:
//            ASF_ERROR(2);
//            return false;
//    }
//}
//
//bool CCommunication::sendCommand_HangXin(int CmdCode,uint8_t* Data,uint16_t Len){
//    serial.Purge();
//    uint8_t err=FALSE;
//    static struct HangXinSendPacket sendPacket;
//    memcpy(sendPacket.fixed,"zt\0",3);
//    memcpy(sendPacket.fixed2,"\0\0\0\0",3);
//    sendPacket.cmd=CmdCode;
//    if((CmdCode>=1&&CmdCode<=0x1f)||CmdCode==0x22){
//        err=TRUE;
//    }
//    if(err!=FALSE){
//        uint8_t* p=(uint8_t*)&sendPacket;
//        sendPacket.sumVal=0;
//        for(int i=2;i<sendPacket.len+12;i++){
//            sendPacket.sumVal+=p[i];
//        }
//        memcpy(p+sendPacket.len+12,&sendPacket.sumVal,2);
//        memcpy(packet,p,sendPacket.len+14);
//
//        DWORD    sendCnt=0;
//        LONG     result=0;
//        if(isUSB()){
//            result=USB_Send(packet,sendPacket.len+14);
//        } else{
//            result=serial.Write(packet,sendPacket.len+14,&sendCnt,NULL,10*1000);
//        }
//        return TRUE;
//    } else{
//        //没有该命令
//        return FALSE;
//    }
//}
//
//bool CCommunication::sendCommand_Default(int CmdCode,uint8_t* Data,uint16_t Len){
//    serial.Purge();
//    uint8_t err=TRUE;
//    struct DataPacketGD32F30 SendPack;
//    SendPack.Head=0x02EF;
//    SendPack.Addr=0x04030201;
//    SendPack.Password=0x01020304;
//    SendPack.NO=0x0201;
//    SendPack.CMD=CmdCode;
//    SendPack.Sign=0x01;
//    SendPack.Length=Len;
//    memcpy(SendPack.Sendbuf,Data,Len);
//    switch(CmdCode){
//        case CMD_GET_TEST_IMAGE:      break;/*获得背景图片*/
//        case CMD_DEVICE_RESET:        break;/*系统复位*/
//        case CMD_DETECT_FINGER:       break;/*探测指纹*/
//        case CMD_GET_RAW_IMAGE:       break;/*采集原始图像*/
//        case CMD_GET_REDRESS_IMAGE:   break;/*采集矫正图像*/
//        case CMD_UPLOAD_IMAGE:        break;/*上传图像(包括原始图像和矫正图像)*/
//        case CMD_GEN_CHAR:            break;/*生成模板*/
//        case CMD_MATCH_CHAR:          break;/*比对模板*/
//        case CMD_STORE_CHAR:          break;/*存储模板*/
//        case CMD_SEARCH_CHAR:         break;/*搜索模板*/
//        case CMD_DELETE_CHAR:         break;/*删除指定模板*/
//        case CMD_EMPTY_CHAR:          break;/*清空模板库*/
//        case CMD_VERIFY_CHAR:         break;/*单一比对模板*/
//        case CMD_GET_CHAR:            break;/*获取模板*/
//        case CMD_PUT_CHAR:            break;/*下载模板*/
//        case CMD_GET_MBINDEX:         break;/*获取MBIndex*/
//        case CMD_READ_NOTE_BOOK:      break;/*读记事本*/
//        case CMD_WRITE_NOTE_BOOK:     break;/*写记事本*/
//        case CMD_READ_PAR_TABLE:      break;/*读取参数表*/
//        case CMD_SET_BAUD_RATE:       break;/*设置波特率*/
//        case CMD_SET_SECURLEVEL:      break;/*设置指纹安全级别*/
//        case CMD_SET_CMOS_PARA:       break;/*设置CMOS参数*/
//        case CMD_RESUME_FACTORY:      break;/*恢复出厂设置*/
//        case CMD_MERGE_CHAR:          break;/*合并特征*/
//        case CMD_SET_PSW:             break;/*密码设置*/
//        case CMD_SET_ADDRESS:         break;/*地址设置*/
//        case CMD_GET_RANDOM:          break;/*获取随机数*/
//        case CMD_DOWNLOAD_IMAGE:      break;/*下载指纹图像*/
//        case CMD_ERASE_PROGRAM:       break;/*擦除程序标识*/
//        case CMD_STORE_CHAR_DIRECT:   break;/*直接存储指纹模板*/
//        case CMD_READ_CHAR_DIRECT:    break;/*直接根据地址读取指纹模板*/
//        case CMD_GET_FIRSTVALID_ADD:  break;/*获取第一个空闲的指纹索引*/
//        case CMD_CHIP_ERASE:          break;/*整片擦除FLASH*/
//        default:err=TRUE;
//    }
//    if(err!=FALSE){
//
//        DWORD    sendCnt=0;
//        LONG     result=0;
//        UINT16   length=SendPack.Length+19;
//        uint8_t* SendPt=(uint8_t *)&SendPack;
//        UINT16   crcValue=GetCRC16(SendPt,length-2);
//        SendPack.Sendbuf[Len]=crcValue&0xFF;
//        SendPack.Sendbuf[Len+1]=(crcValue>>8)&0xFF;
//        memcpy(packet,SendPt,length);
//
//        ////用于获取命令的16进制表示
//        //freopen("1.txt","w",stdout);
//        ////printf("CmdCode:%x\n",CmdCode);
//        //for(int i=0;i<length;i++){
//        //	printf("%c",packet[i]);
//        //}
//
//        if(isUSB()){
//            result=USB_Send(packet,length);
//        } else{
//            result=serial.Write(packet,length,&sendCnt,NULL,10*1000);
//        }
//        return TRUE;
//    } else{
//        //没有该命令
//        return FALSE;
//    }
//}
//
//bool CCommunication::waitForPacket(int timeOutMs){
//    //临时debug代码,设定超时时间为60s
//    timeOutMs=60*1000;
//    LONG ret,time=1;
//    do{
//        if(isUSB()){
//            ret=USB_Receive(packet,sizeof packet);
//        } else{
//            ret=serial.Read(packet,sizeof packet,&packetCnt,0,timeOutMs);
//        }
//        MyLog.print(Log::LOGT,"超时重试第%d次",time++);
//    } while(chkTimeoutContinue->GetCheck()&&ret!=ERROR_SUCCESS);
//    return ret==ERROR_SUCCESS;
//}
//
////清洗串口包的函数,把包头和校验码洗掉,留下来数据放入packetData
//void CCommunication::getDataFromPacket(){
//    packetDataLen=0;
//    switch(cmbChipType->GetCurSel()){
//        case 0://Default
//            for(int i=0,j=0;i<packetCnt;){
//                //包数据部分长度
//                int k=packet[i+15]+packet[i+16]*256;
//                MyLog.print(Log::LOGT,"Default singlePacketLen=%d",k);
//                //兼容以前的bug
//                if(k==530)k=528;
//                //跳过包头
//                i+=19;
//                //累计数据长度
//                packetDataLen+=k;
//                while(k--){
//                    packetData[j++]=packet[i++];
//                }
//                //TODO: CRC校验
//                //跳过CRC
//                i+=2;
//                if(i>packetCnt)
//                    ASF_WARNING(2);
//            }
//            break;
//        case 1://HangXin
//            memcpy(&packetDataLen,packet+8,4);
//            MyLog.print(Log::LOGT,"HangXin packetDataLen=%d",packetDataLen);
//            memcpy(packetData,packet+12,packetDataLen);
//            break;
//    }
//
//}
//
//
//
//BYTE cdb_w[16]={0xef,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//BYTE cdb_r[16]={0xef,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//
//bool CCommunication::USB_Send(BYTE CmdBuf[],DWORD CmdLength){
//    return UsbPort.USBSCSIWrite(UsbPort.m_DeviceHandle,cdb_w,IOCTRL_CDB_LEN,CmdBuf,CmdLength,SCSI_TIMEOUT);
//}
//
//bool CCommunication::USB_Receive(BYTE RspBuf[],DWORD RspLength){
//    return UsbPort.USBSCSIRead(UsbPort.m_DeviceHandle,cdb_r,IOCTRL_CDB_LEN,RspBuf,RspLength,SCSI_TIMEOUT);
//}
