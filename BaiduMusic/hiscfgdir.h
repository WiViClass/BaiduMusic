/*****************************************************************************
	ģ����      : common
	�ļ���      : hiscfgdir.h
	����ļ�    : 
	�ļ�ʵ�ֹ���: ��ȡ�����ļ�����ʷ��¼�ļ����ڵ�·��
	����        : ������
	�汾        : V1.0  Copyright(C) 1997-2015 KDC, All rights reserved.
	-----------------------------------------------------------------------------
	�޸ļ�¼:
	��  ��      �汾        �޸���      �޸�����
	2015/06/25  1.0         ������      ����
*******************************************************************************/
#ifndef HIS_CFG_FILE_DIR_H
#define HIS_CFG_FILE_DIR_H

// ������������
BOOL GetTargetSpecialFolder( TCHAR* pachFolderPath, int nPathBufLen, int nFolderType );
BOOL GetHisCfgFileDir( TCHAR* pachDir, int nDirBufLen );

#endif // HIS_CFG_FILE_DIR_H

#ifdef HIS_CFG_FILE_DIR_CPP  // ����ʵ�ֲ��֣������ΰ����ظ����������

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "windows.h"
#include "Shlobj.h"
#include "version.h"

// ��ȡ ���桢��ʼ->������ ��·��  
// ������int nFolder����ӦCSIDL_COMMON_PROGRAMS��CSIDL_COMMON_DESKTOPDIRECTORY��
BOOL GetTargetSpecialFolder( TCHAR* pachFolderPath, int nPathBufLen, int nFolderType )
{
	LPITEMIDLIST  ppidl;
	if ( SHGetSpecialFolderLocation( NULL, nFolderType, &ppidl ) == S_OK )  	
	{  	
		TCHAR achTargetPath[MAX_PATH*2] = { 0 };
		BOOL bRet = SHGetPathFromIDList( ppidl, achTargetPath ); 
		int nCpyLen = min( nPathBufLen-1, sizeof(achTargetPath)/sizeof(TCHAR)-1 );
		_tcsncpy( pachFolderPath, achTargetPath, nCpyLen );
		pachFolderPath[nCpyLen] = _T('\0');
		CoTaskMemFree( ppidl );  	
		return bRet;  	
	}  

	return FALSE;  	
}

// ��ȡ�����ļ�����ʷ��¼�ļ������������¼���Ĵ��·��(��������\)
// TCHAR* pachDir[in out]�������ļ�����ʷ��¼�ļ��Ĵ��·��
// int nDirBufLen[in]:  pachDir�ĳ��ȣ����ַ������������ֽ���
BOOL GetHisCfgFileDir( TCHAR* pachDir, int nDirBufLen )
{
#ifndef _DEBUG  // Ϊ�˷������Ŀ����͵��ԣ���debug�µ�Ŀ¼����exe��ǰĿ¼��
	// �жϵ�ǰϵͳ�Ƿ���win7����ϵͳ
	BOOL bOsWin7OrAbove = FALSE;
	OSVERSIONINFOEX osvi;
	ZeroMemory( &osvi, sizeof(OSVERSIONINFOEX) );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx( (OSVERSIONINFO*)&osvi );
	/*****************************************************************************
	Operating System             Version     PlatformID
	Windows 8                    6.2         VER_PLATFORM_WIN32_NT (=2)
	Windows 7                    6.1         VER_PLATFORM_WIN32_NT
	Windows Vista                6.0         VER_PLATFORM_WIN32_NT
	Windows XP 64-Bit Edition    5.2         VER_PLATFORM_WIN32_NT
	Windows XP                   5.1         VER_PLATFORM_WIN32_NT
	******************************************************************************/
	// win7��ϵͳ�汾ΪNT6.1 win8Ϊ6.2
	if ( VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && 
		( ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 0 )
		|| osvi.dwMajorVersion > 6 ) )
	{
		bOsWin7OrAbove = TRUE;	
	}

	if ( bOsWin7OrAbove ) // win7������ϵͳ
	{
		// win7������ϵͳ��ʹ��public�ĵ�·����C:\Users\Public\Documents��
		TCHAR achPublicDocPath[MAX_PATH*2] = { 0 };
		BOOL bRet = GetTargetSpecialFolder( achPublicDocPath, sizeof(achPublicDocPath)/sizeof(TCHAR), CSIDL_COMMON_DOCUMENTS );
		if ( bRet )
		{
			_sntprintf_s( pachDir, nDirBufLen, nDirBufLen-1, _T("%s\\%s\\"), achPublicDocPath, PCMT_PRODUCT_ENG_NAME );
			pachDir[nDirBufLen-1] = _T('\0');
			return TRUE;
		}
		else
		{
			assert( bRet == FALSE );
			return FALSE;
		}
	}
	else
#endif
	{
		// ��xpϵͳ�У���ʷ��¼�ļ��������ļ���ά���ڰ�װĿ¼��
		//strCfgHisFileDir = GetModuleFullPath();
		TCHAR achDllName[MAX_PATH] = {0};
		GetModuleFileName( NULL, achDllName, MAX_PATH - 1 );

		TCHAR* pachRet = _tcsrchr( achDllName, _T('\\') ); // �����ҵ���\\��
		if ( pachRet != NULL )
		{
			int nCpyLen = min(pachRet-achDllName, nDirBufLen-2);
			_tcsncpy( pachDir, achDllName, nCpyLen );
			pachDir[nCpyLen] = _T('\\'); // �ֶ����ϡ�\\��
			pachDir[nCpyLen+1] = _T('\0');
			return TRUE;
		}
		else
		{
			assert( pachRet == FALSE );
			return FALSE;
		}
	}
}

#endif // HIS_CFG_FILE_DIR_CPP