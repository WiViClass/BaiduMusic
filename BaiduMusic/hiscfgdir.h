/*****************************************************************************
	模块名      : common
	文件名      : hiscfgdir.h
	相关文件    : 
	文件实现功能: 获取配置文件和历史记录文件所在的路径
	作者        : 张振兴
	版本        : V1.0  Copyright(C) 1997-2015 KDC, All rights reserved.
	-----------------------------------------------------------------------------
	修改记录:
	日  期      版本        修改人      修改内容
	2015/06/25  1.0         张振兴      创建
*******************************************************************************/
#ifndef HIS_CFG_FILE_DIR_H
#define HIS_CFG_FILE_DIR_H

// 函数声明部分
BOOL GetTargetSpecialFolder( TCHAR* pachFolderPath, int nPathBufLen, int nFolderType );
BOOL GetHisCfgFileDir( TCHAR* pachDir, int nDirBufLen );

#endif // HIS_CFG_FILE_DIR_H

#ifdef HIS_CFG_FILE_DIR_CPP  // 函数实现部分，避免多次包含重复定义的问题

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "windows.h"
#include "Shlobj.h"
#include "version.h"

// 获取 桌面、开始->程序组 的路径  
// 参数：int nFolder，对应CSIDL_COMMON_PROGRAMS、CSIDL_COMMON_DESKTOPDIRECTORY等
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

// 获取配置文件和历史记录文件（比如聊天记录）的存放路径(包括最后的\)
// TCHAR* pachDir[in out]：配置文件和历史记录文件的存放路径
// int nDirBufLen[in]:  pachDir的长度，是字符个数，不是字节数
BOOL GetHisCfgFileDir( TCHAR* pachDir, int nDirBufLen )
{
#ifndef _DEBUG  // 为了方便代码的开发和调试，将debug下的目录放在exe当前目录中
	// 判断当前系统是否是win7以上系统
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
	// win7的系统版本为NT6.1 win8为6.2
	if ( VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && 
		( ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 0 )
		|| osvi.dwMajorVersion > 6 ) )
	{
		bOsWin7OrAbove = TRUE;	
	}

	if ( bOsWin7OrAbove ) // win7及以上系统
	{
		// win7及以上系统，使用public文档路径（C:\Users\Public\Documents）
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
		// 在xp系统中，历史记录文件和配置文件还维持在安装目录下
		//strCfgHisFileDir = GetModuleFullPath();
		TCHAR achDllName[MAX_PATH] = {0};
		GetModuleFileName( NULL, achDllName, MAX_PATH - 1 );

		TCHAR* pachRet = _tcsrchr( achDllName, _T('\\') ); // 逆向找到“\\”
		if ( pachRet != NULL )
		{
			int nCpyLen = min(pachRet-achDllName, nDirBufLen-2);
			_tcsncpy( pachDir, achDllName, nCpyLen );
			pachDir[nCpyLen] = _T('\\'); // 手动加上“\\”
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