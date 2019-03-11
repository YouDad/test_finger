#ifndef _xLibCore_H
#define _xLibCore_H
#pragma comment (lib, "xlibCore.lib")
#define def_judge_true(val, vmin, vmax) 	(((val) >= (vmin)) && ((val) <= (vmax)))
#define def_judge_false(val, vmin, vmax) 	(!(def_judge_true(val, vmin, vmax)))

#define def_Feature_maxC			(6)//注册的最大次数，最小没限制
#define def_FingerMaxCount		(200)//存储的指纹模板最多200个，最大1536字节
#define def_finger_size				(1536)//单个最大字节数

#define def_iW		(160)//宽度
#define def_iH		(160)//高度

extern int G_score;//可获取数据，搜索和比对的结果分数，0~几百，越大越好区间，0~50比对失败
extern unsigned int G_coreBuf[], 			// [(90 * 1024) / 4],
										G_Template[], 		// [(def_finger_size) + 3) / 4],1536字节指纹模板
										G_MerMeBuckup[]; 	//

extern void xFA_init(void);//算法初始化main函数入口调用。

extern int xFA_GetsThreshold(int securLevel);//default 3(0<=50, 1:10, 2:25, 3:35, 4:45, 5:50, >5:50) 安全等级获取分数值，1~5，等级越高比对越困难1返回10很容易过，2

extern void xFA_Version(char ver[32]);//获取算法的版本信息，

extern int xFA_FingerDetect(void);//有没有手指，或者指纹能否达到算法要求质量，提示拿开手指，再放，循环，-1值失败，1有手指，0没有手指

extern int xFA_GetImage_Enroll(unsigned int timeout);//100ms for enroll，注册的时候采集图像，timeout默认100，1ok ，0超时，-1失败      a

extern int xFA_GetImage(unsigned int timeout);//100ms for match获取图像，大部分时候使用，在G_coreBuf[]                    b

extern int xFA_ProcessReturnQCV(unsigned char *Q, unsigned char *C, unsigned int *V);//generate not use 不管    比对时使用提取特征，q返回的质量，c特征点的数量，v有效图像面积按像素点算。调试使用。返回1生成 成功，0生成 失败。  e

extern int xFA_GenChar(int nBufferID); //generate char采集指纹后生成指纹特征，6次最多。起始为1，返回值1好，0错        c

extern int xFA_LoadAFinger(unsigned char No);// 0~199最大200个指纹 占时不用 成功返回1，失败返回0，还有个作用直接读取指纹特征G_Template

extern int xFA_MatchAFingerBase(unsigned char sThreshold);//指纹比对  暂时不用  返回1或者返回2都是比对成功（都可以），2是自学习加入，返回0比对失败，小于0数据非法或者错误。参数由21行xFA_GetsThreshold生成的值//与xFA_LoadAFinger配套

extern int xFA_VerifyAFinger(unsigned char No, unsigned char sThreshold);//与指定的模板比对，参数由21行xFA_GetsThreshold生成的值，起始为1，返回值1好，0错

extern int xFA_VerifyAllFinger(unsigned char sThreshold, unsigned char *No);//搜索，sThreshold=xFA_GetsThreshold生成的值，比对成功的编号，最大400~500ms

extern int xFA_SearchRangeFinger(unsigned char sThreshold, unsigned char startNo, int count, unsigned char *N);//范围搜索，第几号开始搜索多少个，

extern int xFA_RegModel(int sameFinger); //merge 3 su合并模板，生成成功1，失败0，是否允许，参数为1，0强制合并。    d

extern int xFA_WriteTemplateToAFinger(unsigned char No);//write su  当45行合并成功xFA_RegModel，1成功，其他值都是失败。

extern void xFA_SetImage(int ok);//down img  为特殊功能，下载图片，直接存图片到image后再调用，1有效，0无效，

extern int xFA_EraseFinger(unsigned char No, int count);//删除指纹，从NO开始的指纹书count，返回1成功，0或负数，

extern int xFA_EraseAllFinger(void);//全部清空  

extern int xFA_CheckLegitimacy(void);//check sunfa valid su  判断目前算法授权是否合法，1合法，0非法

extern int xFA_CheckAFinger(unsigned char No);//检查一下当前NO号的指纹特征是否有效，1当前有指纹，0没写或非法。

extern int xFA_ReadAllFingerCount(void);//获取已存入指纹的个数。

extern int xFA_ReadAuthFactor(unsigned char sn[16]);//read factor    读取授权因子，每个模组的授权号。16字节，1成功，0失败

extern int xFA_StoreAuthCode(const unsigned char AuthCode[256]);//store su  写入授权码256字节，由算法肖工提供成功1，失败0

extern int xFA_DownFinger(unsigned char *finger);//下载指纹模板G_Template，

extern int xFA_RegModelSearchFinger(uint8_t sThreshold, uint8_t startNo, int count1, uint8_t *N);//客户需要的，合并完成成功之后再比对。范围内对比。

extern void xFA_SetMatchBZ(uint8_t bz);//0x66是现在算法的默认值比对的机制，0x88会比对的更精确通过率更好时间更多。非该值会更快但是通过率低。

extern uint8_t xFA_GetMatchBZ();//读取算法的对比机制值xFA_SetMatchBZ

extern void xFA_SetEnrollS(uint8_t s);//设置注册门限，值越高注册越严格。默认是40只能小于等于40，可调用20~30更容易注册

extern uint8_t xFA_GetEnrollS();//获取xFA_SetEnrollS。
int xCapture(unsigned char image[]);//你需要完善这个函数，就是采集图像160*160
									//成功返回1，失败返回0
#endif
