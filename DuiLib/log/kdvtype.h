/*****************************************************************************
   ģ����      : KDV type 
   �ļ���      : kdvtype.h
   ����ļ�    : 
   �ļ�ʵ�ֹ���: KDV�궨��
   ����        : κ�α�
   �汾        : V3.0  Copyright(C) 2001-2002 KDC, All rights reserved.
-----------------------------------------------------------------------------
   �޸ļ�¼:
   ��  ��      �汾        �޸���      �޸�����
   2004/02/17  3.0         κ�α�        ����
   2012/08/21  4.0         ������        ������ض���
******************************************************************************/
#ifndef _KDV_TYPE_H_
#define _KDV_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	  /* Type definition */
/*-----------------------------------------------------------------------------
ϵͳ�����ļ���������Ա�Ͻ��޸�
------------------------------------------------------------------------------*/

/* if vc version < vs2010, not fully support c99 */
#if defined(_MSC_VER) && _MSC_VER < 1600
/* "s8 = signed char" is the right choice, but our apps heavily use s8 as char,
   define as this will cause type conversion compile errors.
   So give up to set "s8 = char".

   This alias is fault in Linux ARM platform, where char == unsigned char
*/
//typedef signed char     s8;
typedef char            s8;
typedef unsigned char   u8;
typedef signed short    s16;
typedef unsigned short  u16;
typedef signed int      s32;
typedef unsigned int    u32;
typedef __int64         s64;
typedef unsigned __int64 u64;

#else //support c99
#include <stdint.h>
typedef char            s8;
/* see comments above */
//typedef int8_t          s8;
typedef uint8_t         u8;
typedef int16_t         s16;
typedef uint16_t        u16;
typedef int32_t         s32;
typedef uint32_t        u32;
typedef int64_t         s64;
typedef uint64_t        u64;
#endif // end of _MSC_VER

typedef s32             BOOL32;

#ifndef _MSC_VER
#ifndef LPSTR
#define LPSTR   char *
#endif
#ifndef LPCSTR
#define LPCSTR  const char *
#endif
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _KDV_def_H_ */

/* end of file kdvdef.h */

