#pragma once
#include "duilib.h"

class CMenuWnd: public CXMLWnd
{
public:
	explicit CMenuWnd(LPCTSTR pszXMLName);

protected:
	virtual ~CMenuWnd();   // 私有化析构函数，这样此对象只能通过new来生成，而不能直接定义变量。就保证了delete this不会出错

public:
	void Init( HWND hWndParent, POINT ptPos);
	void TrackPopupMenu( DWORD dwAlignment, POINT point, HWND hParentWnd );
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	//void SetMainWnd(HWND hWnd);
	
	virtual void    Notify(TNotifyUI& msg);

	void Play();
	void Pause();
	void PlayNext();
	void PlayPrev();
	void SetPlayState(WPARAM wparam,LPARAM lparam);
	void ShowPlayBtn(bool bPlay);

private:
	
	CPaintManagerUI *m_pOwnerPM;
	HWND m_hParentWnd;    // 菜单消息通知窗口句柄
};
