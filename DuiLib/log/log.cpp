/******************************************************************************
	模块名      : pcmtwnd
	文件名      : log.cpp
	相关文件    : log.h
	文件实现功能: 规范打印输出、增加打印开关、打印错误历史
	作者        : 
	版本        : 1.0
	-------------------------------------------------------------------------------
	修改记录:
	日  期      版本        修改人      修改内容
	2012/11/09  1.0         刘晓静      创建
******************************************************************************/
#include "StdAfx.h"
#include "log.h"
#include <shlobj.h>
#include <Shlwapi.h>

// 打印开关
static   BOOL32                   g_bUILogOn = FALSE;

#define  LOG_ERR_DIR              _T("log\\")          // 错误输出目录
#define  LOG_ERR_FILE             _T("_logerr.txt")     // 错误输出文件名

#define  LOG_MAX_OUTPUT_SIZE      512                    // 最大输出字节数
#define  LOG_ERR_MAX_OUTPUT_SIZE  2048                   // 最大错误输出字节数             






// 普通输出打印

void uilog( s8*  pszFmt, ...)
{
	if ( !g_bUILogOn )
	{
		return;
	}

	s8  achPrintBuf[LOG_MAX_OUTPUT_SIZE] = {0};
	s32 nBufLen = sprintf( achPrintBuf, "[ui]" );
   
	// 打印参数列表
    va_list vaArgList;
    va_start( vaArgList, pszFmt );    
    vsprintf( achPrintBuf + nBufLen, pszFmt, vaArgList );   
    va_end( vaArgList );
 
    nBufLen = strlen(achPrintBuf);
	if (nBufLen < LOG_MAX_OUTPUT_SIZE - 1)
	{
		achPrintBuf[nBufLen++]='\n';
	}
	else
	{
		achPrintBuf[LOG_MAX_OUTPUT_SIZE - 1]='\0';
	}
    
	//OspPrintf( TRUE, FALSE, achPrintBuf );
}


//打印开

API void uilogon()
{
    g_bUILogOn = TRUE;
}


// 打印关

API void uilogoff()
{
    g_bUILogOn = FALSE;	
}

// 获取 桌面、开始->程序组 的路径  
// 参数：int nFolder，对应CSIDL_COMMON_PROGRAMS、CSIDL_COMMON_DESKTOPDIRECTORY等
BOOL GetSpecialFolderPath( TCHAR* pszTargetPath, int nFolder )
{  	
	if ( pszTargetPath == NULL )
	{
		return FALSE;
	}

	LPITEMIDLIST  ppidl;  	
	if ( SHGetSpecialFolderLocation( NULL, nFolder, &ppidl ) == S_OK )  	
	{  	
		BOOL bRet = SHGetPathFromIDList( ppidl, pszTargetPath );  	
		CoTaskMemFree( ppidl );  	
		return bRet;  	
	}  

	return FALSE;  	
} 

CDuiString GetModuleFullPath( HINSTANCE g_hPcmtInstance )
{
	TCHAR achDllName[MAX_PATH] = _T("");
	CDuiString csPath = _T("");

	GetModuleFileName( g_hPcmtInstance, achDllName, sizeof (achDllName) );

	csPath = achDllName;
	csPath = csPath.Left( csPath.ReverseFind('\\') + 1 );

	return csPath;
}



// 获取托盘图标的log文件路径
CDuiString GetDuiLogFilePath()
{
	static CDuiString strPath;
	if ( strPath.IsEmpty() )
	{
		TCHAR TrayPath[MAX_PATH] = {0};
		GetSpecialFolderPath( TrayPath ,CSIDL_MYMUSIC );
		//
		strPath = TrayPath;

		SYSTEMTIME st;
		::GetLocalTime( &st );
		//CTime time( st );
		//CDuiString strTime = time.Format( _T("%Y-%m-%d") ); 

		strPath += _T("\\BaiduMusic\\log\\");
		BOOL bPathExsit = PathFileExists( strPath );
		if(!bPathExsit)
		{
			CreateDirectory(strPath,NULL);
		}
		//strPath += strTime;
		strPath += _T("Dui_log.txt");
	}

	return strPath;
}

// 打印托盘图标的相关LOG
void WriteDuiLog( LPCTSTR pszLog )
{
	if ( pszLog == NULL )
	{
		return;
	}

	CDuiString strTrayIconLogPath = GetDuiLogFilePath();
	if ( strTrayIconLogPath.IsEmpty() )
	{
		return;
	}
	WriteLogFunc( strTrayIconLogPath, pszLog );
}


void WriteLogFunc( CDuiString strFilePath, LPCTSTR pszLog )
{
	if ( pszLog == NULL )
	{
		return;
	}

	BOOL bFileExsit = PathFileExists( strFilePath );

	LPCTSTR pszMode = NULL;
#ifdef _UNICODE
	pszMode = _T("ab+"); // 对于Unicode，要向文件中写入Unicode文字，必须以二进制方式打开
#else
	pszMode = _T("a+");
#endif

	FILE* pFile = NULL;
	int err = _tfopen_s( &pFile, strFilePath, pszMode );
	if( ERROR_SUCCESS != err )
	{
		return;
	}

#ifdef _UNICODE
	if ( !bFileExsit )
	{
		// 新创建的日志文件，则写入Unicode头
		BYTE chUnicodeHead[2] = { 0xff, 0xfe }; // Unicode头
		fwrite( chUnicodeHead, sizeof(BYTE), sizeof(chUnicodeHead), pFile );
	}
#endif

	SYSTEMTIME time;
	::GetLocalTime( &time );
	_ftprintf( pFile, _T("%04d-%02d-%02d %02d:%02d:%02d:%03d   [pid:%d,tid:%d][%s]%s\r\n"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, GetCurrentProcessId(), GetCurrentThreadId(), PRODUCT_ENG_NAME, pszLog );
	fclose( pFile );
}