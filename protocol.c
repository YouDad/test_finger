#include "protocol.h"
#include "stdio.h"
#include "string.h"
uint8_t ImageBuf[256*360];
uint8_t    Revbuf[549];
uint8_t    Sendbuf[549];
struct Rev_struct 	rev_stc;//=(struct Rev_struct*)Revbuf;
struct image_struct imageInfo=
{
	//.bufpt =	Sendbuf,
	.bufIsEmpty=Empty,
	.width	= 160,
	.high	= 160,
	.readLine	=0,
	.SendPack=
	{	
		.Head=Pack_Head,
		.ACK=Pack_ACK_sign,
	},
};

struct _Rev_pack_struct Rev_pack;

// struct _Send_pack_struct Send_pack =
// 	{

// 	};
//void Send_struct_init(uint16_t x,uint16_t y)
//{
//	imageInfo =
//	{
//		.bufIsEmpty=Empty,
//		.width	=x,
//		.high	=y,
//		.Length =sizeof(_Send_pack_struct)-4+x;
//		.SendPack=
//		{
//			.Head	=	Pack_Head,
//			.Sign 	= 	Pack_ACK_sign,
//			.datapt	=	Sendbuf,
//			.Length	=	2+x,
//		};
//	};


//}
	
void protocol_deal(struct _Rev_pack_struct *R_pack)
{
//		uint8_t deal_CMD;
	
			imageInfo.SendPack.Head		=	R_pack->rev_stc.Head;
			imageInfo.SendPack.Addr		=	R_pack->rev_stc.Addr;
			imageInfo.SendPack.Password	=	R_pack->rev_stc.Password;
			imageInfo.SendPack.NO		=		R_pack->rev_stc.NO	;
			imageInfo.SendPack.CMD		=		R_pack->rev_stc.CMD;
			imageInfo.SendPack.Sign		=		R_pack->rev_stc.Sign;
			imageInfo.SendPack.Length	=		R_pack->rev_stc.Length;

		switch(R_pack->rev_stc.CMD)
		{
//������					
			

				case CMD_DEVICE_RESET	    	:imageInfo.SendPack.CMD=CMD_DEVICE_RESET	    	;    	break;//ϵͳ��λ
				case CMD_DETECT_FINGER	    :imageInfo.SendPack.CMD=CMD_DETECT_FINGER	    ;	    	break;//̽��ָ��
				case CMD_GET_RAW_IMAGE	    :imageInfo.SendPack.CMD=CMD_GET_RAW_IMAGE	    ;	
																			GET_RAW_IMAGE(&imageInfo);
																			break;//
				case CMD_GET_REDRESS_IMAGE	:imageInfo.SendPack.CMD=CMD_GET_REDRESS_IMAGE	;        	break;//�ɼ�����ͼ��
				case CMD_UPLOAD_IMAGE	    :imageInfo.SendPack.CMD=CMD_UPLOAD_IMAGE	    	;    	break;//�ϴ�ͼ��(����ԭʼͼ��ͽ���ͼ��)
				case CMD_GEN_CHAR	    	:imageInfo.SendPack.CMD=CMD_GEN_CHAR	    	   	;    	break;//	����ģ��
				case CMD_MATCH_CHAR	    	:imageInfo.SendPack.CMD=CMD_MATCH_CHAR	    	;   	    break;//	�ȶ�ģ��
				case CMD_STORE_CHAR	    	:imageInfo.SendPack.CMD=CMD_STORE_CHAR	    	;   	    break;//	�洢ģ��
				case CMD_SEARCH_CHAR	    :imageInfo.SendPack.CMD=CMD_SEARCH_CHAR	    	;   	    break;//	����ģ��
				case CMD_DELETE_CHAR	    :imageInfo.SendPack.CMD=CMD_DELETE_CHAR	    	;   	    break;//	ɾ��ָ��ģ��
				case CMD_EMPTY_CHAR	    	:imageInfo.SendPack.CMD=CMD_EMPTY_CHAR	    	;   	    break;//	���ģ���
				case CMD_VERIFY_CHAR	    :imageInfo.SendPack.CMD=CMD_VERIFY_CHAR	    	;   	    break;//	��һ�ȶ�ģ��
				case CMD_GET_CHAR	    	:imageInfo.SendPack.CMD=CMD_GET_CHAR	    	  	;	    break;//	��ȡģ��
				case CMD_PUT_CHAR	    	:imageInfo.SendPack.CMD=CMD_PUT_CHAR	    	   	;    	break;//	����ģ��
				case CMD_GET_MBINDEX	    :imageInfo.SendPack.CMD=CMD_GET_MBINDEX	    	;   	    break;//	��ȡMBIndex
				case CMD_READ_NOTE_BOOK	    :imageInfo.SendPack.CMD=CMD_READ_NOTE_BOOK	    ;	    break;//�����±�
				case CMD_WRITE_NOTE_BOOK	:imageInfo.SendPack.CMD=CMD_WRITE_NOTE_BOOK	    ;	    break;//	д���±�
				case CMD_READ_PAR_TABLE	    :imageInfo.SendPack.CMD=CMD_READ_PAR_TABLE	    ;	    break;//��ȡ������
				case CMD_SET_BAUD_RATE	    :imageInfo.SendPack.CMD=CMD_SET_BAUD_RATE	    ;	    	break;//���ò�����
				case CMD_SET_SECURLEVEL	    :imageInfo.SendPack.CMD=CMD_SET_SECURLEVEL	    ;	    break;//����ָ�ư�ȫ����
				case CMD_SET_CMOS_PARA	    :imageInfo.SendPack.CMD=CMD_SET_CMOS_PARA	    ;	    	break;//����CMOS����
				case CMD_RESUME_FACTORY	    :imageInfo.SendPack.CMD=CMD_RESUME_FACTORY	    ;	    break;//�ָ���������
				case CMD_MERGE_CHAR	    	:imageInfo.SendPack.CMD=CMD_MERGE_CHAR	    	;	    break;//�ϲ�����
				case CMD_SET_PSW	    	:imageInfo.SendPack.CMD=CMD_SET_PSW	    		;	    break;//	��������
				case CMD_SET_ADDRESS	    :imageInfo.SendPack.CMD=CMD_SET_ADDRESS	    	;	    break;//	��ַ����
				case CMD_GET_RANDOM	    	:imageInfo.SendPack.CMD=CMD_GET_RANDOM	    	;	    break;//��ȡ�����
				case CMD_DOWNLOAD_IMAGE	    :imageInfo.SendPack.CMD=CMD_DOWNLOAD_IMAGE	    ;	    break;//����ָ��ͼ��
				case CMD_ERASE_PROGRAM	    :imageInfo.SendPack.CMD=CMD_ERASE_PROGRAM	    ;	    	break;//���������ʶ
				case CMD_STORE_CHAR_DIRECT	:imageInfo.SendPack.CMD=CMD_STORE_CHAR_DIRECT	;   	    	break;//	ֱ�Ӵ洢ָ��ģ��
				case CMD_READ_CHAR_DIRECT	:imageInfo.SendPack.CMD=CMD_READ_CHAR_DIRECT	   	;    	break;//	ֱ�Ӹ��ݵ�ַ��ȡָ��ģ��
				case CMD_GET_FIRSTVALID_ADD	:imageInfo.SendPack.CMD=CMD_GET_FIRSTVALID_ADD	;   	    break;//	��ȡ��һ�����е�ָ������
				case CMD_CHIP_ERASE	    	:imageInfo.SendPack.CMD=CMD_CHIP_ERASE	    	;	    break;//��Ƭ����FLASH
		}
}


void GET_RAW_IMAGE(struct image_struct *image )
{
	uint32_t time,i;
	uint16_t remainder;
	uint8_t ImageBuf[256*360];
	Sensor_capture_sl(ImageBuf);	
	i=(image->width)*(image->high);
	time=i/(530-2);
	remainder=i%(530-2);
	for(i=0;i<(time+1);i++)
	{
		if(i<time)
		{
			memcpy(&image->SendPack.Sendbuf[2],&ImageBuf[i*528],528);		
			image->SendPack.Length=530;
		}
		else
		{
			memcpy(&image->SendPack.Sendbuf[2],&ImageBuf[i*528],remainder);
			image->SendPack.Length=remainder;
		}
		//CRC16计算
		HexSend((image->SendPack.Length+19),(uint8_t *)&image->SendPack );
	//image->SendPack.crc16=GetCRC16(image->SendPack,(image.Length-2));//crc16
		//HexSend(image.Length,image->SendPack);
	}

}


