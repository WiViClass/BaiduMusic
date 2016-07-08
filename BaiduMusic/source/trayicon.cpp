
#include "..\include\trayicon.h"
#include "..\include\MenuWnd.h"



CTrayIcon::CTrayIcon()   
: m_trayCursorPos( CPoint(0, 0) ) 
, m_hWndTrayRMenu( NULL )
{ 
	memset( &m_tnd, 0, sizeof(m_tnd) ); 
} 

CTrayIcon::CTrayIcon( HWND hWnd,WORD uCallbackMessage, LPCTSTR szToolTip, HICON icon, WORD uID) 
{ 
	Create( hWnd,uCallbackMessage, szToolTip, icon, uID);  
} 

CTrayIcon& CTrayIcon::Instance()
{
	static CTrayIcon s_hInstance;
	return s_hInstance;
}

CTrayIcon::~CTrayIcon()
{
	HideIcon();
	DeleteIcon();
}


BOOL CTrayIcon::Create( HWND hWnd, WORD uCallbackMessage, LPCTSTR szToolTip, HICON hIcon, WORD uID) 
{ 
	//m_pManager = pm;
	BOOL bEnabled = FALSE;

	// 文件只能使用在WINDOW 95以上的版本中 
	bEnabled = ( GetVersion() & 0xff ) >= 4 ; 
	assert( bEnabled );
	if ( !bEnabled ) 
	{
		//uilog( "[CTrayIcon::Create] os version below WINDOW 95" );
		return FALSE; 
	}

	// 确认通知窗口有效 
	ASSERT( bEnabled =  ::IsWindow(hWnd) ); 
	if ( !bEnabled ) 
	{
		//uilog( "[CTrayIcon::Create] handle of main window is invalid" );
		return bEnabled; 
	}

	//确认自定义消息大于WM_USER 
		ASSERT( uCallbackMessage >= WM_USER ); 

	// 确定提示文本长度小于64 
	ASSERT( _tcslen( szToolTip ) <= 127 ); 

	// 定义NOTIFYICONDATA结构的数据项 
	memset( &m_tnd, 0, sizeof(m_tnd) );
	m_tnd.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnd.hWnd = hWnd; //窗口句柄，鼠标放到托盘上时该窗口接受消息
	m_tnd.uID = uID;	//图标标示值（唯一常量）
	m_tnd.hIcon = hIcon; //图标资源
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; //
	m_tnd.uCallbackMessage = uCallbackMessage; 
	_tcsncpy_s ( m_tnd.szTip, 127, szToolTip, _tcslen(szToolTip) );

	// 设置图标 
	bEnabled = Shell_NotifyIcon( NIM_ADD, &m_tnd ); 
	if ( bEnabled )
	{
		//uilog( "[CTrayIcon::Create] Shell_NotifyIcon[NIM_ADD] failed" );

		CDuiString strLog;
		int nErrCode = GetLastError();
		strLog.Format( _T("[CTrayIcon::Create]托盘图标创建失败，GetLastErr：%d"), nErrCode );

	CDuiString sLog;
	 sLog = _T("呵呵");
		//WriteLog( sLog );
		
	}

	return bEnabled;
} 

BOOL CTrayIcon::DeleteIcon() 
{ 
	m_tnd.uFlags = 0; 
	return Shell_NotifyIcon( NIM_DELETE, &m_tnd ); 
} 

BOOL CTrayIcon::HideIcon() 
{  
	m_tnd.uFlags = NIF_ICON; 
	return Shell_NotifyIcon( NIM_DELETE, &m_tnd ); 
} 

BOOL CTrayIcon::ShowIcon() 
{ 
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
	return Shell_NotifyIcon( NIM_ADD, &m_tnd ); 
} 

BOOL CTrayIcon::SetIcon( HICON hIcon ) 
{ 
	m_tnd.uFlags = NIF_ICON; 
	m_tnd.hIcon = hIcon; 
	BOOL bRet = Shell_NotifyIcon( NIM_MODIFY, &m_tnd ); 
	if ( !bRet )
	{
		CDuiString strLog;
		int nErrCode = GetLastError();
		strLog.Format( _T("[CTrayIcon::SetIcon]托盘图标设置失败，GetLastErr：%d"), nErrCode );
	//	WriteLog( strLog );
	}

	return bRet;
} 

BOOL CTrayIcon::SetIcon( LPCTSTR lpszIconName ) 
{  
	HICON hIcon = ::LoadIcon( g_hPcmtInstance, lpszIconName );
	return SetIcon( hIcon ); 
} 

BOOL CTrayIcon::SetIcon( WORD nIDResource ) 
{ 
	HICON hIcon = ::LoadIcon( g_hPcmtInstance, MAKEINTRESOURCE(nIDResource) );
	return SetIcon( hIcon ); 
} 

HICON CTrayIcon::GetIcon() const 
{ 
	HICON hIcon = NULL; 
	hIcon = m_tnd.hIcon;
	return hIcon; 
} 

BOOL CTrayIcon::SetTooltipText( LPCTSTR pszTip ) 
{ 
	m_tnd.uFlags = NIF_TIP; 
	_tcscpy( m_tnd.szTip, pszTip ); 

	return Shell_NotifyIcon( NIM_MODIFY, &m_tnd ); 
} 

CDuiString CTrayIcon::GetTooltipText() const 
{ 
	CDuiString strText; 
	strText = m_tnd.szTip; 
	return strText; 
} 

bool CTrayIcon::SetNotificationWnd( HWND hWnd ) 
{ 
	if ( !::IsWindow( hWnd ))
	{
		return false;
	}

	m_tnd.hWnd = hWnd; 
	m_tnd.uFlags = 0; 

	return Shell_NotifyIcon( NIM_MODIFY, &m_tnd ); 
} 

HWND CTrayIcon::GetNotificationWnd() const 
{ 
	return  m_tnd.hWnd; 
} 

LRESULT CTrayIcon::OnIconNotification( WPARAM wParam, LPARAM lParam ,bool IsReadyPlay) 
{ 
	if ( WM_MOUSEMOVE == lParam )
	{
		GetCursorPos( &m_trayCursorPos );

		// 如果是右键单击或左键双击，则在处理消息的过程中不应该显示消息盒子提示框的

        // 当托盘图标有右键菜单显示时，不能同时显示消息盒子窗口，所以在右键菜单显示时不显示消息盒子
		// 而由于DUI菜单不像MFC中的菜单，MFC的菜单显示调用TrackPopupMenu，从显示到用户点击一直是阻
		// 塞式的，DUI菜单的TrackPopupMenu是将窗口显示出来，显示出来后立即返回，即非阻塞式的，所以
		// 此处要通过检测保存的m_hWndTrayRMenu句柄来判断右键菜单是否存在，by zzx 2014/04/23
		// 防止窗口句柄重复使用，所以加上菜单窗口类名的过滤
		TCHAR achClassName[MAX_PATH] = { 0 };
		::GetClassName( m_hWndTrayRMenu, achClassName, sizeof(achClassName)/sizeof(TCHAR)-1 );

		BOOL bShowFlag = FALSE;
		if ( !IsWindow( m_hWndTrayRMenu ) ) // 句柄对应的窗口已经不存在，要显示
		{
			bShowFlag = TRUE;
		}
		else // 句柄对应的窗口依然存在
		{
			// 句柄对应的窗口不再是CMenuWnd菜单窗口类，此时也要显示
			if ( _tcscmp( achClassName, _T("CMenuWnd") ) )
			{
				bShowFlag = TRUE;
			}
		}

		if ( bShowFlag )
		{
			//// 有未查看的消息发起者时，显示消息盒子提示界面
			//if ( CMsgBoxLogic::Instance()->GetMsgLauncherNum() > 0 )
			//{
			//	CMsgBoxTipDlg::Instance()->MoveWndPos();
			//	CMsgBoxTipDlg::Instance()->ShowWindow( SW_SHOW );
			//	::SetTimer( g_pMainLogic->GetMainHwnd(), TRACK_MSGBOX_TIP_DLG_STATUS, 300, NULL );
			//}
		}

		return 0;
	}

	if ( wParam != m_tnd.uID ) 
	{
		return 0; 
	}

	// 单击右键弹出菜单 
	if ( WM_RBUTTONUP == LOWORD(lParam) ) 
	{ 	

		// 加上这一句，点其它的地方，托盘菜单才会消失
		::SetForegroundWindow( m_tnd.hWnd );

		// 动态创建菜单
		CMenuWnd *pTrayMenu = NULL;

		if ( IsReadyPlay)
		{
			pTrayMenu = new CMenuWnd( _T("popup_menu_tray.xml"));
		}
		else
		{
			pTrayMenu = new CMenuWnd( _T("popup_menu_tray_quit.xml"));
		}

		CPoint pt;
		GetCursorPos( &pt );
		//pTrayMenu->Init(m_tnd.hWnd , pt);
		//pTrayMenu->ShowWindow(TRUE);
		pTrayMenu->TrackPopupMenu( 1, pt, m_tnd.hWnd );
		//pTrayMenu->SetMainWnd(m_tnd.hWnd)
		//pTrayMenu->ShowWindow(TRUE);

		//m_hWndTrayRMenu = pTrayMenu->GetHWND();
	} 
	else if ( WM_LBUTTONDOWN == LOWORD(lParam) || WM_LBUTTONDBLCLK == LOWORD(lParam) ) 
	{ 
		//if( CMsgBoxTipDlg::Instance()->GetHWND() != NULL )
		//{
		//	CMsgBoxTipDlg::Instance()->ShowWindow( SW_HIDE );
		//}

		//::KillTimer( g_pMainLogic->GetMainHwnd(), TRACK_MSGBOX_TIP_DLG_STATUS );

		//static BOOL bClickChatWnd = FALSE;
		//BOOL bFind = CMsgBoxLogic::Instance()->FindAndShowMsgUnreadDlg();
		//if ( bFind )
		//{
		//	bClickChatWnd = TRUE;
		//	return 1;
		//}

		BOOL fResult;
		CDuiRect rtWorkArea;
		fResult = SystemParametersInfo( SPI_GETWORKAREA, 0, &rtWorkArea, 0 );//获取工作区区域
		ASSERT( fResult );
	//	CDuiFrameWnd* pMainWnd = (CDuiFrameWnd*)g_pMainLogic->GetMainFrameWnd();
	//	if ( NULL != pMainWnd->GetHWND() )
		//{  
			//if ( !pMainWnd->IsWindowVisible() || pMainWnd->IsIconic() )
			//{
			//	pMainWnd->ShowWindow( SW_RESTORE );
			//}

			//if ( bClickChatWnd )
			//{
			//	bClickChatWnd = FALSE;
			//}

			//if ( SIDEHIDE_NONE == pMainWnd->GetSideHideMode() )
			//{
			//	pMainWnd->SetForegroundWindow();
			//} 
			//else if ( pMainWnd->GetSideHideMode() != SIDEHIDE_NONE )
			//{
			//	// 如果窗口处于贴边掩藏状态，双击后应该将窗口伸展显示在屏幕上
			//	CDuiRect rectWnd;
			//	pMainWnd->GetWindowRect( &rectWnd );
			//	EmSideHideMode emSideHideMode = pMainWnd->GetSideHideMode();
			//	if ( SIDEHIDE_TOP == emSideHideMode )
			//	{
			//		s32 nHeight = rectWnd.Height();
			//		rectWnd.top = rtWorkArea.top - 2;
			//		rectWnd.bottom = rectWnd.top + nHeight;
			//		pMainWnd->MoveWindow( &rectWnd );
			//	}
			//	else if( SIDEHIDE_LEFT == emSideHideMode )
			//	{
			//		s32 nWidth = rectWnd.Width();
			//		rectWnd.left = rtWorkArea.left - 2;
			//		rectWnd.right = rectWnd.left + nWidth;
			//		pMainWnd->MoveWindow( &rectWnd );
			//	}
			//	else if ( SIDEHIDE_RIGHT == emSideHideMode )
			//	{
			//		s32 nWidth = rectWnd.Width();
			//		rectWnd.right = rtWorkArea.right + 2;
			//		rectWnd.left = rectWnd.right - nWidth;
			//		pMainWnd->MoveWindow( &rectWnd );
			//	}
			//	else if ( SIDEHIDE_BUTTOM == emSideHideMode )
			//	{
			//		s32 nHeight = rectWnd.Height();
			//		rectWnd.bottom = rtWorkArea.bottom + 2;
			//		rectWnd.top = rectWnd.bottom - nHeight;
			//		pMainWnd->MoveWindow( &rectWnd );
			//	}

			//	pMainWnd->SetForegroundWindow();
			//}
		//}
	}

	return 1; 
} 

CPoint CTrayIcon::GetTrayCursorPos()
{
	return m_trayCursorPos;
}

VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{

}

void CTrayIcon::PlayAni()
{
	//::SetTimer(m_pManager->GetPaintWindow(), LOGIN_ICON_PLAY, 200, TimerProc );
}

bool CTrayIcon::Stop()
{
	//::KillTimer( m_pManager->GetPaintWindow(), LOGIN_ICON_PLAY );
	return true;
}