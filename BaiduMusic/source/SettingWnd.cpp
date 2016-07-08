#if 0
#include "..\include\SettingWnd.h"

void SettingWnd::Init(HWND hWndParent, POINT ptPos)
{
	if( hWndParent == NULL ) 
	{
		return;
	}

	//	m_pOwnerPM = pOwnerPM;
	Create(hWndParent, _T("setting_wnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	::ClientToScreen(hWndParent, &ptPos);
	//::GetParent(hWndParent)
	::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}
void SettingWnd::OnClick(TNotifyUI& msg)
{
	CDuiString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("menu_setting") )
	{
		//Close();
		//return; 
	}



	__super::OnClick(msg);
}
void SettingWnd::OnFinalMessage( HWND /*hWnd*/ )
{
	delete this;
}

LRESULT SettingWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg )
	{
	case WM_KILLFOCUS:    
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
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

LRESULT SettingWnd::OnKillFocus( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	Close();
	bHandled = FALSE;
	return __super::OnKillFocus(uMsg, wParam, lParam, bHandled); 
}
#endif