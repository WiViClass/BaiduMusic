#include "..\include\MenuWnd.h"
#include "..\include\DuiFrameWnd.h"
CMenuWnd::CMenuWnd( LPCTSTR pszXMLName ) 
	: CXMLWnd(pszXMLName),
	m_hParentWnd( NULL ),
	m_pOwnerPM(NULL)
{

}

CMenuWnd::~CMenuWnd()
{

}

void CMenuWnd::Init(HWND hWndParent, POINT ptPos)
{
	if( hWndParent == NULL ) 
	{
		return;
	}

//	m_pOwnerPM = pOwnerPM;
	Create(hWndParent, _T("main_menu"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	::ClientToScreen(hWndParent, &ptPos);
	::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CMenuWnd::TrackPopupMenu( DWORD dwAlignment, POINT point, HWND hParentWnd )
{
	m_hParentWnd = hParentWnd;
	Create( m_hParentWnd, NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE, CDuiRect() );

	::PostMessage(m_hParentWnd, WM_MY_TRAY_SHOW_MENU, (WPARAM) GetHWND(), (LPARAM) TRUE);

	
	//  POINT ptPos = 
	::SetWindowPos(*this, NULL, point.x - 182, point.y - 310, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
	 ::ShowWindow(m_hWnd,SW_SHOW);
}

void CMenuWnd::OnClick(TNotifyUI& msg)
{

	CDuiString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("menu_setting") )
	{
		CDuiFrameWnd *pSettingWnd = new CDuiFrameWnd(_T("setting_wnd.xml"));
		pSettingWnd->Create(NULL, _T("BaiduMusic"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
		pSettingWnd->CenterWindow();
		Close();

	}else if(sCtrlName == _T("btn_prev_tray_menu"))
	{
		PlayPrev();
	}else if(sCtrlName == _T("btn_next_tray_menu"))
	{
		PlayNext();
	}else if(sCtrlName == _T("btn_play_tray_menu"))
	{
		Play();
	}else if(sCtrlName == _T("btn_pause_tray_menu"))
	{
		Pause();
	}

	//Close();
	__super::OnClick(msg);
}
void CMenuWnd::OnFinalMessage( HWND /*hWnd*/ )
{
	delete this;
}

LRESULT CMenuWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg )
	{
	case WM_KILLFOCUS:    
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
		break; 

	case WM_MY_MUSIC_STATE:    
		//lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
		SetPlayState(wParam ,lParam);
		break; 

	default:
		bHandled = FALSE;
	}

	if(bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMenuWnd::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Close();
	bHandled = FALSE;
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled); 
}

void CMenuWnd::Notify( TNotifyUI& msg )
{
	if(msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		if(m_pOwnerPM)
		{
			m_pOwnerPM->SendNotify(msg);
		}

		Close();
	}

	__super::Notify(msg); 
}

void CMenuWnd::Play()
{
	::SendMessage(m_hParentWnd, WM_MY_MUSIC_PLAY, (WPARAM) TRUE, (LPARAM) TRUE);
	ShowPlayBtn(true);
}
void CMenuWnd::Pause()
{
	::SendMessage(m_hParentWnd, WM_MY_MUSIC_PAUSE, (WPARAM) TRUE, (LPARAM) TRUE);
	ShowPlayBtn(false);
}
void CMenuWnd::PlayNext()
{
	::SendMessage(m_hParentWnd, WM_MY_MUSIC_NEXT, (WPARAM) TRUE, (LPARAM) TRUE);
}
void CMenuWnd::PlayPrev()
{
	::SendMessage(m_hParentWnd, WM_MY_MUSIC_PREV, (WPARAM) TRUE, (LPARAM) TRUE);
}

void CMenuWnd::ShowPlayBtn(bool bPlay)
{
	 CButtonUI *pbtnTrayPlay  =static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_play_tray_menu")));

	 CButtonUI *pbtnTrayPause = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_pause_tray_menu")));
	if(pbtnTrayPlay && pbtnTrayPause)
	{
		pbtnTrayPlay->SetVisible(! bPlay);
		pbtnTrayPause->SetVisible(bPlay);
	}
}

void CMenuWnd::SetPlayState(WPARAM wparam,LPARAM lparam)
{
	ShowPlayBtn((bool)wparam);
	CDuiString &PlayTitle = (CDuiString &)lparam;
	CTextUI *m_pTextSongName=  static_cast<CTextUI*>(m_PaintManager.FindControl(_T("tray_playing_song")));
	//CDuiString tmp = PlayTitle;
	m_pTextSongName->SetText(PlayTitle);

}
