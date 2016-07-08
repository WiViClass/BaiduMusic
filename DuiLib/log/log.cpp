/******************************************************************************
	ģ����      : pcmtwnd
	�ļ���      : log.cpp
	����ļ�    : log.h
	�ļ�ʵ�ֹ���: �淶��ӡ��������Ӵ�ӡ���ء���ӡ������ʷ
	����        : 
	�汾        : 1.0
	-------------------------------------------------------------------------------
	�޸ļ�¼:
	��  ��      �汾        �޸���      �޸�����
	2012/11/09  1.0         ������      ����
******************************************************************************/
#include "StdAfx.h"
#include "log.h"
#include <shlobj.h>
#include <Shlwapi.h>

// ��ӡ����
static   BOOL32                   g_bUILogOn = FALSE;

#define  LOG_ERR_DIR              _T("log\\")          // �������Ŀ¼
#define  LOG_ERR_FILE             _T("_logerr.txt")     // ��������ļ���

#define  LOG_MAX_OUTPUT_SIZE      512                    // �������ֽ���
#define  LOG_ERR_MAX_OUTPUT_SIZE  2048                   // ����������ֽ���             






// ��ͨ�����ӡ

void uilog( s8*  pszFmt, ...)
{
	if ( !g_bUILogOn )
	{
		return;
	}

	s8  achPrintBuf[LOG_MAX_OUTPUT_SIZE] = {0};
	s32 nBufLen = sprintf( achPrintBuf, "[ui]" );
   
	// ��ӡ�����б�
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


//��ӡ��

API void uilogon()
{
    g_bUILogOn = TRUE;
}


// ��ӡ��

API void uilogoff()
{
    g_bUILogOn = FALSE;	
}

// ��ȡ ���桢��ʼ->������ ��·��  
// ������int nFolder����ӦCSIDL_COMMON_PROGRAMS��CSIDL_COMMON_DESKTOPDIRECTORY��
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



// ��ȡ����ͼ���log�ļ�·��
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

// ��ӡ����ͼ������LOG
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
	pszMode = _T("ab+"); // ����Unicode��Ҫ���ļ���д��Unicode���֣������Զ����Ʒ�ʽ��
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
		// �´�������־�ļ�����д��Unicodeͷ
		BYTE chUnicodeHead[2] = { 0xff, 0xfe }; // Unicodeͷ
		fwrite( chUnicodeHead, sizeof(BYTE), sizeof(chUnicodeHead), pFile );
	}
#endif

	SYSTEMTIME time;
	::GetLocalTime( &time );
	_ftprintf( pFile, _T("%04d-%02d-%02d %02d:%02d:%02d:%03d   [pid:%d,tid:%d][%s]%s\r\n"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, GetCurrentProcessId(), GetCurrentThreadId(), PRODUCT_ENG_NAME, pszLog );
	fclose( pFile );
}