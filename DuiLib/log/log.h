/******************************************************************************
	模块名      : pcmtwnd
	文件名      : log.h
	相关文件    : log.cpp
	文件实现功能: 规范打印输出、增加打印开关、打印错误历史
	作者        : 
	版本        : 1.0
	-------------------------------------------------------------------------------
	修改记录:
	日  期      版本        修改人      修改内容
	2012/11/09  1.0         刘晓静      创建
******************************************************************************/

#pragma once
//#include <UIlib.h>
#include "kdvtype.h"

using namespace DuiLib;

#define API extern "C"          __declspec(dllexport)
#define COMMERCIAL_VERSION          0 

// 普通输出打印
void uilog(s8*  pszFmt, ...);

// 错误输出打印
#define uilogerr uilogerrhead(__FILE__, __LINE__);uilogerrbody

// 打印错误头
void uilogerrhead(s8*  pszFile, s32 nLine);
// 打印错误内容
void uilogerrbody(TCHAR*  pszFmt, ...);

// 打印开
API void uilogon();

// 打印关
API void uilogoff();


BOOL GetSpecialFolderPath( TCHAR* pszTargetPath, int nFolder );

// 获取托盘图标的log文件路径
CDuiString GetDuiLogFilePath();
// 打印托盘图标的相关LOG
UILIB_API void  WriteDuiLog( LPCTSTR pszLog );




void WriteLogFunc( CDuiString strFilePath, LPCTSTR pszFormat );


//////////////////////////////////////////////////////////
// 宏控制打印函数，商用版本不打印，可以提高运行效率///////
//////////////////////////////////////////////////////////


inline void _format( TCHAR *szLog, int nLen, LPCTSTR pszFormat, ...)
{
	va_list argList;
	va_start( argList, pszFormat );
	::_vstprintf_s( szLog, nLen, pszFormat, argList );
	va_end( argList );
}

// 格式化参数，并输出到func，其中nArgs==0的时候不格式化参数，直接输出
#define FormatAndWriteOut( func, pszFormat, nArgs, ... )\
{\
	if ( nArgs == 0)func(pszFormat);\
	else{\
	TCHAR szLog[0x1000]={0};\
    _format( szLog, 0x1000, pszFormat, __VA_ARGS__ );\
	func(szLog);\
    }\
}


// 最多允许32个参数。
/****************************************************************************************************************************
* 可能有人看不懂这个宏函数，在此说明下。
* C语言的函数参数是可以不定的，用...来表示。如何计算出这个可变参数的个数呢？有两种方案，一种利用c++0x的统一初始化列表，如下：
* Y_TUPLE_SIZE(...) (sizeof(int[]{__VA_ARGS__}) / sizeof(int)) // 这个会自动构造匿名对象int数组，即可求出长度。
* 因为vs2010对c++0x支持的还不算完整，故采用以下方式，Y_TUPLE_SIZE(...)语句后面有跟一个连接字符串，以及32-0这33个整数，
* 并特化当没有参数的情况，这就是 Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX
* 我们可以简单的看下没有参数的情况，此时__VA_ARGS__代表为空，因此Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX 就替换为
* Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX，于是 Y_TUPLE_SIZE_II(__args) 就展开为
* Y_TUPLE_SIZE_I(Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
* Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX宏再次展开为,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
* 此时Y_TUPLE_SIZE_I传参为Y_TUPLE_SIZE_I（,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0）。前面32个逗号分别代表前面32个参数，第33个参数就是__n = 0(前面一大串逗号其实是把参数省了，写不写都无所谓)
* 以上就是对无参数的处理，如果有参数就是一般的展开，比如一个参数3，Y_TUPLE_SIZE_PREFIX_3_Y_TUPLE_SIZE_POSTFIX 就是普通参数.这样
* Y_TUPLE_SIZE_I的参数就是Y_TUPLE_SIZE_PREFIX_3_Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
* 此时第33个参数就是1.
* 如果是两个参数如(3,4)那么__VA_ARGS__会替换为3,4则展开为Y_TUPLE_SIZE_PREFIX_3,4_Y_TUPLE_SIZE_POSTFIX 这样总共就是34个参数了，于是__n就是2
* 以此类推，可以得到最多3数2个参。
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


