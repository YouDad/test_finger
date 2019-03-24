#pragma once
#include"stdafx.h"

/*
警告或错误前面如果有:
~:随着工程的进行,可能需要发生变化
!:随着工程的进行,不太可能发生变化
有 没有序号的警告和错误(模板)
可以复制一个新的,然后再改上一个
*/

#ifdef ASF_WARNING
#error macro is repeated!
#else
#define ASF_WARNING(ID) \
	log(LOGW,"%s at %s(%d)FuncName:<%s>",ASF_WARNING##ID,__FILE__,__LINE__,__FUNCTION__)
#endif

//WARNING STRING
#ifdef ASF_WARNING01
#error macro is repeated!
#else
#define ASF_WARNING01 \
"~好像调用了过于高级的log"
#endif

#ifdef ASF_WARNING02
#error macro is repeated!
#else
#define ASF_WARNING02 \
"~数据包格式不完全正确"
#endif

#ifdef ASF_WARNING03
#error macro is repeated!
#else
#define ASF_WARNING03 \
"~图片像素个数不是160*160个"
#endif

#ifdef ASF_WARNING04
#error macro is repeated!
#else
#define ASF_WARNING04 \
"!位图对象句柄为0x00"
#endif

#ifdef ASF_WARNING05
#error macro is repeated!
#else
#define ASF_WARNING05 \
"!串口关闭返回值不是ERROR_SUCCESS"
#endif

#ifdef ASF_WARNING__
#error macro is repeated!
#else
#define ASF_WARNING__ \
""
#endif

#ifdef ASF_ERROR
#error macro is repeated!
#else
#define ASF_ERROR(ID) \
	log(LOGE,"%s at %s(%d)FuncName:<%s>",ASF_ERROR##ID,__FILE__,__LINE__,__FUNCTION__)
#endif

#ifdef ASF_ERROR01
#error macro is repeated!
#else
#define ASF_ERROR01 \
"!试图保存文件时失败"
#endif

#ifdef ASF_ERROR__
#error macro is repeated!
#else
#define ASF_ERROR__ \
""
#endif