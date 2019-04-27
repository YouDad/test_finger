#include "stdafx.h"

#pragma comment(lib, "setupapi.lib")

#pragma comment(lib, "hid.lib")

CUsbPort UsbPort;

CUsbPort::CUsbPort(void){
    m_DeviceHandle=NULL;
    //	m_pOwner = NULL;
}

CUsbPort::~CUsbPort(void){
    if(m_DeviceHandle!=NULL){
        CloseHandle(m_DeviceHandle);
    }
}

int CUsbPort::InitUsbPort(int CommType,char *pDesc){
    if(CommType==COMM_USB_MASS){
        if(0==OpenUsbMsc(pDesc)){
            return 0;
        } else{
            return -1;
        }
    } else if(CommType==COMM_USB_HID){
        PHIDP_PREPARSED_DATA	PreparsedData;
        HANDLE PnPHandle;
        struct _GUID GUID;
        SP_INTERFACE_DEVICE_DATA DeviceInterfaceData;
        struct{
            DWORD cbSize;
            char DevicePath[256];
        } FunctionClassDeviceData;
        HIDD_ATTRIBUTES HIDAttributes;
        SECURITY_ATTRIBUTES SecurityAttributes;
        int Device;
        ULONG BytesReturned;
        bool Success,ManufacturerName,ProductName;
        WCHAR ManufacturerBuffer[256],ProductBuffer[256];
        const WCHAR NotSupplied[]=L"NULL";

        // Initialize the GUID array and setup the security attributes for Win2000
        HidD_GetHidGuid(&GUID);
        SecurityAttributes.nLength=sizeof(SECURITY_ATTRIBUTES);
        SecurityAttributes.lpSecurityDescriptor=NULL;
        SecurityAttributes.bInheritHandle=false;

        // Get a handle for the Plug and Play node and request currently active devices
        PnPHandle=SetupDiGetClassDevs(&GUID,NULL,NULL,DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
        if(int(PnPHandle)==-1){
            printf("Could not attach to PnP node");
            int iRetErr=GetLastError();
            return -1;
        }

        // Lets look for a maximum of 20 Devices
        for(Device=0; (Device<20); Device++){
            // Initialize our data
            DeviceInterfaceData.cbSize=sizeof(DeviceInterfaceData);
            // Is there a device at this table entry
            Success=SetupDiEnumDeviceInterfaces(PnPHandle,NULL,&GUID,Device,&DeviceInterfaceData);
            if(Success){

                // There is a device here, get it's name
                FunctionClassDeviceData.cbSize=5;
                Success=SetupDiGetDeviceInterfaceDetail(PnPHandle,&DeviceInterfaceData,
                    (PSP_INTERFACE_DEVICE_DETAIL_DATA)&FunctionClassDeviceData,256,&BytesReturned,NULL);
                if(!Success){
                    printf("Could not find the system name for this device\n");
                    int iRetErr=GetLastError();
                    return -1;
                }


                // Can now open this device
                m_DeviceHandle=CreateFileA(FunctionClassDeviceData.DevicePath,GENERIC_READ|GENERIC_WRITE,
                                           FILE_SHARE_READ|FILE_SHARE_WRITE,&SecurityAttributes,OPEN_EXISTING,0,NULL);
                if(m_DeviceHandle==INVALID_HANDLE_VALUE){
                    printf("Could not open HID #%d, Errorcode = %d\n",Device,GetLastError());
                } else{

#if	1
                    // Get the information about this HID
                    Success=HidD_GetAttributes(m_DeviceHandle,&HIDAttributes);
                    if(!Success){
                        printf("Could not get HID attributes\n");
                        return -1; //GetLastError(); 
                    }
                    ManufacturerName=HidD_GetManufacturerString(m_DeviceHandle,ManufacturerBuffer,256);
                    ProductName=HidD_GetProductString(m_DeviceHandle,ProductBuffer,256);

                    // And display it!
                    printf("VID = %4.4x, Name = ",HIDAttributes.VendorID);
                    printf("%ws, ",ManufacturerName?ManufacturerBuffer:NotSupplied);
                    printf("PID = %4.4x, Name = ",HIDAttributes.ProductID);
                    printf("%ws\n",ProductName?ProductBuffer:NotSupplied);
#endif

                    Success=HidD_GetPreparsedData(m_DeviceHandle,&PreparsedData);
                    HidP_GetCaps(PreparsedData,&Capabilities);

                    if((HIDAttributes.VendorID==0x2009)&&(HIDAttributes.ProductID==0x7638)){
                        return 0;
                    } else{
                        CloseHandle(m_DeviceHandle);

                    }

                }
            } // if (SetupDiEnumDeviceInterfaces . .
        } // for (Device = 0; (Device < 20); Device++)
        //SetupDiDestroyDeviceInfoList(PnPHandle);
    } else if(CommType==COMM_USB_DRIVER){
        if(0==OpenUsbDriver(pDesc)){
            return 0;
        } else{
            return -1;
        }
    }
    return -1;
}



bool CUsbPort::USBSCSIRead(HANDLE hHandle,BYTE* pCDB,DWORD nCDBLen,BYTE* pData,DWORD nLength,DWORD nTimeOut){

    SCSI_PASS_THROUGH_WITH_BUFFERS sptwb;
    ULONG returned,length;
    bool status;

    // Get nand information
    //SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;
    ZeroMemory(&sptwb,sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS));
    sptwb.Spt.Length=sizeof(SCSI_PASS_THROUGH);
    sptwb.Spt.PathId=0;
    sptwb.Spt.TargetId=1;
    sptwb.Spt.Lun=0;
    sptwb.Spt.CdbLength=IOCTRL_CDB_LEN;
    sptwb.Spt.SenseInfoLength=14;
    sptwb.Spt.DataIn=SCSI_IOCTL_DATA_IN;
    sptwb.Spt.DataTransferLength=nLength;
    sptwb.Spt.TimeOutValue=nTimeOut;
    sptwb.Spt.DataBufferOffset=
        offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,DataBuf);
    sptwb.Spt.SenseInfoOffset=
        offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,SenseBuf);
    //	sptdwb.sptd.Cdb[0] = CMD_READ;

    //memcpy( sptwb.Spt.Cdb, CDBBuffer, IOCTRL_CDB_LEN );
    memcpy(sptwb.Spt.Cdb,pCDB,IOCTRL_CDB_LEN);

    // 格式化Nand的FTL区域比较慢
    //	if( sptwb.Spt.Cdb[1] == 0x05 )
    //	{
    //		sptwb.Spt.TimeOutValue		= 30;
    //	}

    length=offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,DataBuf)+
        sptwb.Spt.DataTransferLength;

    status=DeviceIoControl(hHandle,
                           IOCTL_SCSI_PASS_THROUGH,
                           &sptwb,
                           sizeof(SCSI_PASS_THROUGH),
                           &sptwb,
                           length,
                           &returned,
                           false);

    DWORD dwErrCode=GetLastError();
    if(!status){
        return false;
    } else{
        memcpy(pData,sptwb.DataBuf,nLength);
        return true;
    }
}

bool CUsbPort::USBSCSIWrite(HANDLE hHandle,BYTE* pCDB,DWORD nCDBLen,BYTE* pData,DWORD nLength,DWORD nTimeOut){
    ULONG returned,length;
    DWORD dwErrCode;
    bool status;

    //	ASSERT(pData);

    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;
    ZeroMemory(&sptdwb,sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));
    sptdwb.sptd.Length=sizeof(SCSI_PASS_THROUGH_DIRECT);
    sptdwb.sptd.PathId=0;
    sptdwb.sptd.TargetId=1;
    sptdwb.sptd.Lun=0;
    sptdwb.sptd.CdbLength=IOCTRL_CDB_LEN;
    sptdwb.sptd.SenseInfoLength=14;
    sptdwb.sptd.DataIn=SCSI_IOCTL_DATA_OUT;
    sptdwb.sptd.DataTransferLength=nLength;
    sptdwb.sptd.TimeOutValue=nTimeOut;
    sptdwb.sptd.DataBuffer=pData;
    sptdwb.sptd.SenseInfoOffset=
        offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,SenseBuf);
    //    sptdwb.sptd.Cdb[0] = CMD_WRITE; //VENDOR_CMD
    memcpy(sptdwb.sptd.Cdb,pCDB,IOCTRL_CDB_LEN);


    length=sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);

    status=DeviceIoControl(hHandle,
                           IOCTL_SCSI_PASS_THROUGH_DIRECT,
                           &sptdwb,
                           length,
                           &sptdwb,
                           length,
                           &returned,
                           false);


    dwErrCode=GetLastError();


    if(!status)
        return false;
    else
        return true;
}


bool CUsbPort::USBHidWrite(HANDLE hHandle,BYTE* pData,DWORD nLength,DWORD nTimeOut){
    BYTE *OutputReport=NULL;
    OutputReport=(BYTE *)malloc(Capabilities.OutputReportByteLength);
    USHORT HidPackSize;
    HidPackSize=Capabilities.OutputReportByteLength-1;

    DWORD n;
    DWORD l;
    DWORD i;
    bool Success;
    DWORD ErrorNnr;
    DWORD	bWritten;

    OutputReport[0]=0x00;

    n=nLength/HidPackSize;
    l=nLength%HidPackSize;

    for(i=0; i<n; i++){
        memcpy(&OutputReport[1],&pData[i*HidPackSize],HidPackSize);
        Success=WriteFile(hHandle,OutputReport,Capabilities.OutputReportByteLength,&bWritten,NULL);
        ErrorNnr=GetLastError();
    }

    if(l!=0){
        memset(&OutputReport[1],0x00,HidPackSize);
        memcpy(&OutputReport[1],&pData[n*HidPackSize],l);
        Success=WriteFile(hHandle,OutputReport,Capabilities.OutputReportByteLength,&bWritten,NULL);
        ErrorNnr=GetLastError();
    }

    free(OutputReport);

    return true;

}

bool CUsbPort::USBHidRead(HANDLE hHandle,BYTE* pData,DWORD nLength,DWORD nTimeOut){
    BYTE *InputReport=NULL;
    InputReport=(BYTE *)malloc(Capabilities.InputReportByteLength);
    USHORT HidPackSize;
    HidPackSize=Capabilities.InputReportByteLength-1;

    DWORD bRead;
    bool Result;
    DWORD	ErrorNnr;
    DWORD offset=0;

    Result=ReadFile(hHandle,InputReport,Capabilities.InputReportByteLength,&bRead,NULL);
    ErrorNnr=GetLastError();

    //caculate the length
    DWORD Length;
    Length=*((DWORD *)&InputReport[9]);
    if((Length+14)<=HidPackSize){
        memcpy(pData,&InputReport[1],(Length+14));
        return true;
    }

    memcpy(pData,&InputReport[1],HidPackSize);
    offset+=HidPackSize;

    //read oher bytes 
    DWORD LeftLength;
    LeftLength=(Length+14)-HidPackSize;
    DWORD n;
    DWORD l;
    n=LeftLength/HidPackSize;
    l=LeftLength%HidPackSize;
    DWORD i;
    for(i=0; i<n; i++){
        Result=ReadFile(hHandle,InputReport,Capabilities.InputReportByteLength,&bRead,NULL);
        ErrorNnr=GetLastError();
        memcpy(pData+offset,&InputReport[1],HidPackSize);
        offset+=HidPackSize;
    }

    Result=ReadFile(hHandle,InputReport,Capabilities.InputReportByteLength,&bRead,NULL);
    ErrorNnr=GetLastError();
    memcpy(pData+offset,&InputReport[1],l);

    free(InputReport);

    return true;
}


int CUsbPort::CloseUsbPort(void){
    if(m_DeviceHandle!=NULL){
        CloseHandle(m_DeviceHandle);
        m_DeviceHandle=NULL;
    }

    return 0;
}

int CUsbPort::OpenUsbMsc(char *pDesc){
    HDEVINFO        hDevInfo,hIntDevInfo;
    DWORD           index;
    bool            status;

    //
    // Open the device using device interface registered by the driver
    //

    //
    // Get the interface device information set that contains all devices of event class.
    //


    if(0==strcmp(pDesc,"CD")){
        hDevInfo=SetupDiGetClassDevs(
            (LPGUID)&GUID_DEVCLASS_CDROM,
            NULL,
            NULL,
            DIGCF_PRESENT); // All devices present on system
        if(hDevInfo==INVALID_HANDLE_VALUE){
            //DebugPrint(1, "SetupDiGetClassDevs failed with error: %d\n", GetLastError());
            return false;
        }

        hIntDevInfo=SetupDiGetClassDevs(
            (LPGUID)&CdRomClassGuid,
            NULL,                                   // Enumerator
            NULL,                                   // Parent Window
            (DIGCF_PRESENT|DIGCF_INTERFACEDEVICE  // Only Devices present & Interface class
             ));

        if(hDevInfo==INVALID_HANDLE_VALUE){
            //DebugPrint(1, "SetupDiGetClassDevs failed with error: %d\n", GetLastError());
            return false;
        }
    } else if(0==strcmp(pDesc,"UD")){
        hDevInfo=SetupDiGetClassDevs(
            (LPGUID)&GUID_DEVCLASS_DISKDRIVE,
            NULL,
            NULL,
            DIGCF_PRESENT); // All devices present on system
        if(hDevInfo==INVALID_HANDLE_VALUE){
            //DebugPrint(1, "SetupDiGetClassDevs failed with error: %d\n", GetLastError());
            return false;
        }

        hIntDevInfo=SetupDiGetClassDevs(
            (LPGUID)&DiskClassGuid,//CdRomClassGuid,//
            NULL,                                   // Enumerator
            NULL,                                   // Parent Window
            (DIGCF_PRESENT|DIGCF_INTERFACEDEVICE  // Only Devices present & Interface class
             ));

        if(hDevInfo==INVALID_HANDLE_VALUE){
            //DebugPrint(1, "SetupDiGetClassDevs failed with error: %d\n", GetLastError());
            return false;
        }
    } else{
        return false;
    }

    //
    //  Enumerate all the Disk devices
    //
    index=0;

    //	m_nWriteRetryTimes = 0;
    //	CloseAllScsiDevice();
    //	m_arrDeviceHandle.RemoveAll();
    DWORD dwDeviceHandle=0;
    while(true){
        dwDeviceHandle=0;
        status=OpenScsiDiskDevice(hIntDevInfo,index,&dwDeviceHandle,pDesc);
        if(status==false){
            break;
        } else{
            if(dwDeviceHandle){
                break;
            }

        }
        index++;
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    SetupDiDestroyDeviceInfoList(hIntDevInfo);

    if(dwDeviceHandle==0){
        return -1;
    }

    m_DeviceHandle=(HANDLE)dwDeviceHandle;
    return 0;
}

int CUsbPort::OpenScsiDiskDevice(HDEVINFO IntDevInfo,DWORD Index,DWORD* pDeviceHandle,char *pDesc){
    SP_DEVICE_INTERFACE_DATA            interfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA    interfaceDetailData=NULL;
    STORAGE_PROPERTY_QUERY              query;
    PSTORAGE_ADAPTER_DESCRIPTOR         adpDesc;
    PSTORAGE_DEVICE_DESCRIPTOR          devDesc;
    SCSI_PASS_THROUGH_WITH_BUFFERS      sptwb;
    HANDLE                              hDevice;
    bool                                status;
    PUCHAR                              p;
    UCHAR                               outBuf[512];
    UCHAR                               cdTypeString[20];
    ULONG                               length=0,
        returned=0,
        returnedLength,
        cdType=0;
    DWORD                         interfaceDetailDataSize,
        reqSize,
        errorCode,
        i;


    interfaceData.cbSize=sizeof(SP_INTERFACE_DEVICE_DATA);

    if(0==strcmp(pDesc,"CD")){
        status=SetupDiEnumDeviceInterfaces(
            IntDevInfo,              // Interface Device Info handle
            0,                       // Device Info data
            (LPGUID)&CdRomClassGuid,
            Index,                   // Member
            &interfaceData           // Device Interface Data
        );

        if(status==false){
            errorCode=GetLastError();
            if(errorCode==ERROR_NO_MORE_ITEMS){
                //DebugPrint(2, "\nNo more interfaces\n");
            } else{
                //DebugPrint(1, "SetupDiEnumDeviceInterfaces failed with error: %d\n", errorCode);
            }
            return false;
        }
    } else if(0==strcmp(pDesc,"UD")){
        status=SetupDiEnumDeviceInterfaces(
            IntDevInfo,              // Interface Device Info handle
            0,                       // Device Info data
            (LPGUID)&DiskClassGuid,
            Index,                   // Member
            &interfaceData           // Device Interface Data
        );

        if(status==false){
            errorCode=GetLastError();
            if(errorCode==ERROR_NO_MORE_ITEMS){
                //DebugPrint(2, "\nNo more interfaces\n");
            } else{
                //DebugPrint(1, "SetupDiEnumDeviceInterfaces failed with error: %d\n", errorCode);
            }
            return false;
        }
    } else{
        return false;
    }

    //
    // Find out required buffer size, so pass NULL 
    //

    status=SetupDiGetDeviceInterfaceDetail(
        IntDevInfo,         // Interface Device info handle
        &interfaceData,     // Interface data for the event class
        NULL,               // Checking for buffer size
        0,                  // Checking for buffer size
        &reqSize,           // Buffer size required to get the detail data
        NULL                // Checking for buffer size
    );

    //
    // This call returns ERROR_INSUFFICIENT_BUFFER with reqSize 
    // set to the required buffer size. Ignore the above error and
    // pass a bigger buffer to get the detail data
    //

    if(status==false){
        errorCode=GetLastError();
        if(errorCode!=ERROR_INSUFFICIENT_BUFFER){
            //DebugPrint(1, "SetupDiGetDeviceInterfaceDetail failed with error: %d\n", errorCode);
            return false;
        }
    }

    //
    // Allocate memory to get the interface detail data
    // This contains the devicepath we need to open the device
    //

    interfaceDetailDataSize=reqSize;
    interfaceDetailData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(interfaceDetailDataSize);
    if(interfaceDetailData==NULL){
        //DebugPrint(1, "Unable to allocate memory to get the interface detail data.\n");
        return false;
    }
    interfaceDetailData->cbSize=sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

    status=SetupDiGetDeviceInterfaceDetail(
        IntDevInfo,               // Interface Device info handle
        &interfaceData,           // Interface data for the event class
        interfaceDetailData,      // Interface detail data
        interfaceDetailDataSize,  // Interface detail data size
        &reqSize,                 // Buffer size required to get the detail data
        NULL);                    // Interface device info

    if(status==false){
        //DebugPrint(1, "Error in SetupDiGetDeviceInterfaceDetail failed with error: %d\n", GetLastError());
        return false;
    }

    //
    // Now we have the device path. Open the device interface
    // to send Pass Through command

    //DebugPrint(2, "Interface: %s\n", interfaceDetailData->DevicePath);

    hDevice=CreateFile(
        interfaceDetailData->DevicePath,    // device interface name
        GENERIC_READ|GENERIC_WRITE,       // dwDesiredAccess
        FILE_SHARE_READ|FILE_SHARE_WRITE, // dwShareMode
        NULL,                               // lpSecurityAttributes
        OPEN_EXISTING,                      // dwCreationDistribution
        0,                                  // dwFlagsAndAttributes
        NULL                                // hTemplateFile
    );

    //
    // We have the handle to talk to the device. 
    // So we can release the interfaceDetailData buffer
    //

    free(interfaceDetailData);

    if(hDevice==INVALID_HANDLE_VALUE){
        //DebugPrint(1, "CreateFile failed with error: %d\n", GetLastError());
        errorCode=GetLastError();
        return true;
    }

    query.PropertyId=StorageAdapterProperty;
    query.QueryType=PropertyStandardQuery;

    status=DeviceIoControl(
        hDevice,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(STORAGE_PROPERTY_QUERY),
        &outBuf,
        512,
        &returnedLength,
        NULL
    );
    if(!status){
        //DebugPrint(1, "IOCTL failed with error code%d.\n\n", GetLastError());
    } else{
        adpDesc=(PSTORAGE_ADAPTER_DESCRIPTOR)outBuf;
        //DebugPrint(1, "\nAdapter Properties\n");
        //DebugPrint(1, "------------------\n");
        //DebugPrint(1, "Bus Type       : %s\n", BusType[adpDesc->BusType]);
        //DebugPrint(1, "Max. Tr. Length: 0x%x\n", adpDesc->MaximumTransferLength);
        //DebugPrint(1, "Max. Phy. Pages: 0x%x\n", adpDesc->MaximumPhysicalPages);
        //DebugPrint(1, "Alignment Mask : 0x%x\n", adpDesc->AlignmentMask);

        cdType=GetMediaType(hDevice,cdTypeString);

        query.PropertyId=StorageDeviceProperty;
        query.QueryType=PropertyStandardQuery;

        status=DeviceIoControl(
            hDevice,
            IOCTL_STORAGE_QUERY_PROPERTY,
            &query,
            sizeof(STORAGE_PROPERTY_QUERY),
            &outBuf,
            512,
            &returnedLength,
            NULL
        );
        if(!status){
            //DebugPrint(1, "IOCTL failed with error code%d.\n\n", GetLastError());
        } else{
            //DebugPrint(1, "\nDevice Properties\n");
            //DebugPrint(1, "-----------------\n");
            devDesc=(PSTORAGE_DEVICE_DESCRIPTOR)outBuf;
            //
            // Our device table can handle only 16 devices.
            //
            //DebugPrint(1, "SCSI Device Type: %s (0x%X)\n",DeviceType[devDesc->DeviceType > 0x0F ? 0x0F : devDesc->DeviceType], devDesc->DeviceType);
            if(cdType){
                //DebugPrint(1, "Media Type      : %s (0x%x)\n", cdTypeString, cdType);
            }

            if(devDesc->DeviceTypeModifier){
                //DebugPrint(1, "Device Modifier : 0x%x\n", devDesc->DeviceTypeModifier);
            }

            //DebugPrint(1, "Removable Media : %s\n", devDesc->RemovableMedia ? "Yes" : "No");
            p=(PUCHAR)outBuf;

            if(devDesc->VendorIdOffset && p[devDesc->VendorIdOffset]){
                //DebugPrint(1, "Vendor ID       : ");
                for(i=devDesc->VendorIdOffset; p[i]!=(UCHAR)NULL && i<returnedLength; i++){
                    //DebugPrint(1, "%c", p[i]);
                }
                //DebugPrint(1, "\n");
            }
            if(devDesc->ProductIdOffset && p[devDesc->ProductIdOffset]){
                //DebugPrint(1, "Product ID      : ");
                for(i=devDesc->ProductIdOffset; p[i]!=(UCHAR)NULL && i<returnedLength; i++){
                    //DebugPrint(1, "%c", p[i]);
                }
                //DebugPrint(1, "\n");
            }

            if(devDesc->ProductRevisionOffset && p[devDesc->ProductRevisionOffset]){
                //DebugPrint(1, "Product Revision: ");
                for(i=devDesc->ProductRevisionOffset; p[i]!=(UCHAR)NULL && i<returnedLength; i++){
                    //DebugPrint(1, "%c", p[i]);
                }
                //DebugPrint(1, "\n");
            }

            if(devDesc->SerialNumberOffset && p[devDesc->SerialNumberOffset]){
                //DebugPrint(1, "Serial Number   : ");
                for(i=devDesc->SerialNumberOffset; p[i]!=(UCHAR)NULL && i<returnedLength; i++){
                    //DebugPrint(1, "%c", p[i]);
                }
                //DebugPrint(1, "\n");
            }
        }
    }

    // Query
    ZeroMemory(&sptwb,sizeof(SCSI_PASS_THROUGH_WITH_BUFFERS));

    sptwb.Spt.Length=sizeof(SCSI_PASS_THROUGH);
    sptwb.Spt.PathId=0;
    sptwb.Spt.TargetId=1;
    sptwb.Spt.Lun=0;
    sptwb.Spt.CdbLength=CDB6GENERIC_LENGTH;
    sptwb.Spt.SenseInfoLength=24;
    sptwb.Spt.DataIn=SCSI_IOCTL_DATA_IN;
    sptwb.Spt.DataTransferLength=192;
    sptwb.Spt.TimeOutValue=2;
    sptwb.Spt.Cdb[0]=SCSIOP_INQUIRY;
    sptwb.Spt.Cdb[4]=192;
    sptwb.Spt.DataBufferOffset=
        offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,DataBuf);
    sptwb.Spt.SenseInfoOffset=
        offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,SenseBuf);

    length=offsetof(SCSI_PASS_THROUGH_WITH_BUFFERS,DataBuf)+
        sptwb.Spt.DataTransferLength;

    status=DeviceIoControl(hDevice,
                           IOCTL_SCSI_PASS_THROUGH,
                           &sptwb,
                           sizeof(SCSI_PASS_THROUGH),
                           &sptwb,
                           length,
                           &returned,
                           false);


    //PrintStatusResults(status, returned, &sptwb);

    // 需要考虑一级和二级Bootloader弹出的磁盘
    char szVendorID[32];
    char szProductID[32];
    memset(szVendorID,0x00,sizeof(szVendorID));
    memset(szProductID,0x00,sizeof(szProductID));
    memcpy(szVendorID,&sptwb.DataBuf[8],8);
    memcpy(szProductID,&sptwb.DataBuf[16],16);


    if(strstr(szVendorID,"STM     ")&&
       strstr(szProductID,"SD Flash Disk   ")){
        *pDeviceHandle=(DWORD)hDevice;
    } else{
        //
        // Close handle the driver
        //
        if(!CloseHandle(hDevice)){
            //DebugPrint(2, "Failed to close device.\n");
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////

    return true;
}


ULONG CUsbPort::GetMediaType(HANDLE hDevice,PUCHAR cdTypeString){

    PGET_MEDIA_TYPES    mediaTypes;
    bool                status=0;
    UCHAR               buffer[2048];   // Must be big enough hold DEVICE_MEDIA_INFO
    ULONG               returned;

    //
    // Get the Media type.
    //

    status=DeviceIoControl(hDevice,
                           IOCTL_STORAGE_GET_MEDIA_TYPES_EX,
                           NULL,
                           0,
                           buffer,
                           sizeof(buffer),
                           &returned,
                           false);

    if(!status){
        //		DebugPrint(2, "IOCTL_STORAGE_GET_MEDIA_TYPES_EX failed with error code%d.\n\n", GetLastError());
        return 0;
    }

    mediaTypes=(PGET_MEDIA_TYPES)buffer;

    switch(mediaTypes->DeviceType){

        case FILE_DEVICE_CD_ROM:
            strcpy_s((char*)cdTypeString,7,"CD-ROM");
            break;

        case FILE_DEVICE_DVD:
            strcpy_s((char*)cdTypeString,4,"DVD");
            break;

        default:
            strcpy_s((char*)cdTypeString,8,"Unknown");
            break;
    }
    return mediaTypes->DeviceType;
}

GUID GUID_USBINTERFACE={0x96c3c0dc,0x8936,0x4150,0x87,0x00,0x78,0x1e,0x5c,0x7c,0xe5,0xc1};

int CUsbPort::OpenUsbDriver(char *pDesc){
    HDEVINFO                 hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA deviceInfoData;
    ULONG                    i;
    BOOLEAN                  done;
    LPGUID					 pGuid;
    HANDLE					 hOut;
    int ret;

    pGuid=(LPGUID)&GUID_USBINTERFACE;
    hardwareDeviceInfo=SetupDiGetClassDevs(
        pGuid,
        NULL, // Define no enumerator (global)
        NULL, // Define no
        (DIGCF_PRESENT| // Only Devices present
         DIGCF_DEVICEINTERFACE)); // Function class devices.


    deviceInfoData.cbSize=sizeof(SP_DEVICE_INTERFACE_DATA);
    i=0;
    done=false;
    while(!done){
        if(SetupDiEnumDeviceInterfaces(hardwareDeviceInfo,0,pGuid,i,&deviceInfoData)){
            hOut=OpenOneDevice(hardwareDeviceInfo,&deviceInfoData,pDesc);
            if(hOut!=INVALID_HANDLE_VALUE){
                m_DeviceHandle=hOut;
                done=true;
                ret=0;
            } else{
                i++;
            }
        } else{
            done=true;
            ret=-1;
        }
    }

    return ret;
}

HANDLE CUsbPort::OpenOneDevice(HDEVINFO HardwareDeviceInfo,PSP_DEVICE_INTERFACE_DATA DeviceInfoData,char *pDesc){
    PSP_DEVICE_INTERFACE_DETAIL_DATA     functionClassDeviceData=NULL;
    ULONG                                predictedLength=0;
    ULONG                                requiredLength=0;
    HANDLE                               hOut=INVALID_HANDLE_VALUE;

    //
    // allocate a function class device data structure to receive the
    // goods about this particular device.
    //
    SetupDiGetDeviceInterfaceDetail(
        HardwareDeviceInfo,
        DeviceInfoData,
        NULL, // probing so no output buffer yet
        0, // probing so output buffer length of zero
        &requiredLength,
        NULL); // not interested in the specific dev-node


    predictedLength=requiredLength;
    // sizeof (SP_FNCLASS_DEVICE_DATA) + 512;

    functionClassDeviceData=(PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(predictedLength);
    functionClassDeviceData->cbSize=sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    //
    // Retrieve the information from Plug and Play.
    //
    if(!SetupDiGetDeviceInterfaceDetail(
        HardwareDeviceInfo,
        DeviceInfoData,
        functionClassDeviceData,
        predictedLength,
        &requiredLength,
        NULL)){
        free(functionClassDeviceData);
        return INVALID_HANDLE_VALUE;
    }

    static char _temp[1024]={};
    size_t t;
    wcstombs_s(&t,_temp,functionClassDeviceData->DevicePath,1024);

    if(strstr(_temp,pDesc)!=NULL){
        hOut=CreateFile(
            functionClassDeviceData->DevicePath,
            GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE,
            NULL, // no SECURITY_ATTRIBUTES structure
            OPEN_EXISTING, // No special create flags
            0, // No special attributes
            NULL); // No template file
    }

    //if (CheckVidPid(functionClassDeviceData->DevicePath, GDUSB_VID, GDUSB_PID) == 1)
    //{
    //	hOut = CreateFile(
    //		functionClassDeviceData->DevicePath,
    //		GENERIC_READ | GENERIC_WRITE,
    //		FILE_SHARE_READ | FILE_SHARE_WRITE,
    //		NULL, // no SECURITY_ATTRIBUTES structure
    //		OPEN_EXISTING, // No special create flags
    //		0, // No special attributes
    //		NULL); // No template file
    //}

    free(functionClassDeviceData);
    return hOut;
}