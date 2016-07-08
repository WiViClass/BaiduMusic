
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

	// �ļ�ֻ��ʹ����WINDOW 95���ϵİ汾�� 
	bEnabled = ( GetVersion() & 0xff ) >= 4 ; 
	assert( bEnabled );
	if ( !bEnabled ) 
	{
		//uilog( "[CTrayIcon::Create] os version below WINDOW 95" );
		return FALSE; 
	}

	// ȷ��֪ͨ������Ч 
	ASSERT( bEnabled =  ::IsWindow(hWnd) ); 
	if ( !bEnabled ) 
	{
		//uilog( "[CTrayIcon::Create] handle of main window is invalid" );
		return bEnabled; 
	}

	//ȷ���Զ�����Ϣ����WM_USER 
		ASSERT( uCallbackMessage >= WM_USER ); 

	// ȷ����ʾ�ı�����С��64 
	ASSERT( _tcslen( szToolTip ) <= 127 ); 

	// ����NOTIFYICONDATA�ṹ�������� 
	memset( &m_tnd, 0, sizeof(m_tnd) );
	m_tnd.cbSize = sizeof(NOTIFYICONDATA); 
	m_tnd.hWnd = hWnd; //���ھ�������ŵ�������ʱ�ô��ڽ�����Ϣ
	m_tnd.uID = uID;	//ͼ���ʾֵ��Ψһ������
	m_tnd.hIcon = hIcon; //ͼ����Դ
	m_tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; //
	m_tnd.uCallbackMessage = uCallbackMessage; 
	_tcsncpy_s ( m_tnd.szTip, 127, szToolTip, _tcslen(szToolTip) );

	// ����ͼ�� 
	bEnabled = Shell_NotifyIcon( NIM_ADD, &m_tnd ); 
	if ( bEnabled )
	{
		//uilog( "[CTrayIcon::Create] Shell_NotifyIcon[NIM_ADD] failed" );

		CDuiString strLog;
		int nErrCode = GetLastError();
		strLog.Format( _T("[CTrayIcon::Create]����ͼ�괴��ʧ�ܣ�GetLastErr��%d"), nErrCode );

	CDuiString sLog;
	 sLog = _T("�Ǻ�");
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
		strLog.Format( _T("[CTrayIcon::SetIcon]����ͼ������ʧ�ܣ�GetLastErr��%d"), nErrCode );
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

		// ������Ҽ����������˫�������ڴ�����Ϣ�Ĺ����в�Ӧ����ʾ��Ϣ������ʾ���

        // ������ͼ�����Ҽ��˵���ʾʱ������ͬʱ��ʾ��Ϣ���Ӵ��ڣ��������Ҽ��˵���ʾʱ����ʾ��Ϣ����
		// ������DUI�˵�����MFC�еĲ˵���MFC�Ĳ˵���ʾ����TrackPopupMenu������ʾ���û����һֱ����
		// ��ʽ�ģ�DUI�˵���TrackPopupMenu�ǽ�������ʾ��������ʾ�������������أ���������ʽ�ģ�����
		// �˴�Ҫͨ����Ᵽ���m_hWndTrayRMenu������ж��Ҽ��˵��Ƿ���ڣ�by zzx 2014/04/23
		// ��ֹ���ھ���ظ�ʹ�ã����Լ��ϲ˵����������Ĺ���
		TCHAR achClassName[MAX_PATH] = { 0 };
		::GetClassName( m_hWndTrayRMenu, achClassName, sizeof(achClassName)/sizeof(TCHAR)-1 );

		BOOL bShowFlag = FALSE;
		if ( !IsWindow( m_hWndTrayRMenu ) ) // �����Ӧ�Ĵ����Ѿ������ڣ�Ҫ��ʾ
		{
			bShowFlag = TRUE;
		}
		else // �����Ӧ�Ĵ�����Ȼ����
		{
			// �����Ӧ�Ĵ��ڲ�����CMenuWnd�˵������࣬��ʱҲҪ��ʾ
			if ( _tcscmp( achClassName, _T("CMenuWnd") ) )
			{
				bShowFlag = TRUE;
			}
		}

		if ( bShowFlag )
		{
			//// ��δ�鿴����Ϣ������ʱ����ʾ��Ϣ������ʾ����
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

	// �����Ҽ������˵� 
	if ( WM_RBUTTONUP == LOWORD(lParam) ) 
	{ 	

		// ������һ�䣬�������ĵط������̲˵��Ż���ʧ
		::SetForegroundWindow( m_tnd.hWnd );

		// ��̬�����˵�
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
		fResult = SystemParametersInfo( SPI_GETWORKAREA, 0, &rtWorkArea, 0 );//��ȡ����������
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
			//	// ������ڴ��������ڲ�״̬��˫����Ӧ�ý�������չ��ʾ����Ļ��
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