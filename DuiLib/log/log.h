/******************************************************************************
	ģ����      : pcmtwnd
	�ļ���      : log.h
	����ļ�    : log.cpp
	�ļ�ʵ�ֹ���: �淶��ӡ��������Ӵ�ӡ���ء���ӡ������ʷ
	����        : 
	�汾        : 1.0
	-------------------------------------------------------------------------------
	�޸ļ�¼:
	��  ��      �汾        �޸���      �޸�����
	2012/11/09  1.0         ������      ����
******************************************************************************/

#pragma once
//#include <UIlib.h>
#include "kdvtype.h"

using namespace DuiLib;

#define API extern "C"          __declspec(dllexport)
#define COMMERCIAL_VERSION          0 

// ��ͨ�����ӡ
void uilog(s8*  pszFmt, ...);

// ���������ӡ
#define uilogerr uilogerrhead(__FILE__, __LINE__);uilogerrbody

// ��ӡ����ͷ
void uilogerrhead(s8*  pszFile, s32 nLine);
// ��ӡ��������
void uilogerrbody(TCHAR*  pszFmt, ...);

// ��ӡ��
API void uilogon();

// ��ӡ��
API void uilogoff();


BOOL GetSpecialFolderPath( TCHAR* pszTargetPath, int nFolder );

// ��ȡ����ͼ���log�ļ�·��
CDuiString GetDuiLogFilePath();
// ��ӡ����ͼ������LOG
UILIB_API void  WriteDuiLog( LPCTSTR pszLog );




void WriteLogFunc( CDuiString strFilePath, LPCTSTR pszFormat );


//////////////////////////////////////////////////////////
// ����ƴ�ӡ���������ð汾����ӡ�������������Ч��///////
//////////////////////////////////////////////////////////


inline void _format( TCHAR *szLog, int nLen, LPCTSTR pszFormat, ...)
{
	va_list argList;
	va_start( argList, pszFormat );
	::_vstprintf_s( szLog, nLen, pszFormat, argList );
	va_end( argList );
}

// ��ʽ���������������func������nArgs==0��ʱ�򲻸�ʽ��������ֱ�����
#define FormatAndWriteOut( func, pszFormat, nArgs, ... )\
{\
	if ( nArgs == 0)func(pszFormat);\
	else{\
	TCHAR szLog[0x1000]={0};\
    _format( szLog, 0x1000, pszFormat, __VA_ARGS__ );\
	func(szLog);\
    }\
}


// �������32��������
/****************************************************************************************************************************
* �������˿���������꺯�����ڴ�˵���¡�
* C���Եĺ��������ǿ��Բ����ģ���...����ʾ����μ��������ɱ�����ĸ����أ������ַ�����һ������c++0x��ͳһ��ʼ���б����£�
* Y_TUPLE_SIZE(...) (sizeof(int[]{__VA_ARGS__}) / sizeof(int)) // ������Զ�������������int���飬����������ȡ�
* ��Ϊvs2010��c++0x֧�ֵĻ������������ʲ������·�ʽ��Y_TUPLE_SIZE(...)�������и�һ�������ַ������Լ�32-0��33��������
* ���ػ���û�в��������������� Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX
* ���ǿ��Լ򵥵Ŀ���û�в������������ʱ__VA_ARGS__����Ϊ�գ����Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX ���滻Ϊ
* Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX������ Y_TUPLE_SIZE_II(__args) ��չ��Ϊ
* Y_TUPLE_SIZE_I(Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
* Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX���ٴ�չ��Ϊ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
* ��ʱY_TUPLE_SIZE_I����ΪY_TUPLE_SIZE_I��,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0����ǰ��32�����ŷֱ����ǰ��32����������33����������__n = 0(ǰ��һ�󴮶�����ʵ�ǰѲ���ʡ�ˣ�д��д������ν)
* ���Ͼ��Ƕ��޲����Ĵ�������в�������һ���չ��������һ������3��Y_TUPLE_SIZE_PREFIX_3_Y_TUPLE_SIZE_POSTFIX ������ͨ����.����
* Y_TUPLE_SIZE_I�Ĳ�������Y_TUPLE_SIZE_PREFIX_3_Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
* ��ʱ��33����������1.
* ���������������(3,4)��ô__VA_ARGS__���滻Ϊ3,4��չ��ΪY_TUPLE_SIZE_PREFIX_3,4_Y_TUPLE_SIZE_POSTFIX �����ܹ�����34�������ˣ�����__n����2
* �Դ����ƣ����Եõ����3��2���Ρ�
****************************************************************************************************************************/
#ifdef C++11
#define Y_TUPLE_SIZE(...) (sizeof(int[]{__VA_ARGS__}) / sizeof(int))

#else
#define Y_TUPLE_SIZE(...) Y_TUPLE_SIZE_II((Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define Y_TUPLE_SIZE_II(__args) Y_TUPLE_SIZE_I __args

#define Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0

#define Y_TUPLE_SIZE_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n
#endif


#define WriteLog( pszLog, ...) FormatAndWriteOut( WriteDuiLog, pszLog, Y_TUPLE_SIZE(__VA_ARGS__), __VA_ARGS__ );


