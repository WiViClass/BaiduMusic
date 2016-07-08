#pragma once
#include "duilib.h"


class CTrayIcon
{ 
public: 
	~CTrayIcon(); 
	static CTrayIcon& Instance();

public: 
	// 创建系统图标 
	BOOL Create( HWND hWnd,  WORD uCallbackMessage, LPCTSTR szTip, HICON hIcon, WORD uID);

	// 获取NOTIFYICONDATA信息
	NOTIFYICONDATA GetNotifyIconData(){ return m_tnd; }

	// 关于提示文本 
	BOOL SetTooltipText( LPCTSTR pszTooltipText );  
	CDuiString GetTooltipText() const; 

	// 关于图标 
	BOOL SetIcon( HICON hIcon ); 
	BOOL SetIcon( LPCTSTR lpIconName ); 
	BOOL SetIcon( WORD nIDResource ); 
	HICON GetIcon() const; 
	BOOL HideIcon(); 
	BOOL ShowIcon(); 
	BOOL DeleteIcon(); 

	// 关于通知窗口 
	bool SetNotificationWnd( HWND NotifyWnd ); 
	HWND GetNotificationWnd() const; 

	CPoint GetTrayCursorPos();

	// 自定义消息处理函数 
	virtual LRESULT OnIconNotification( WPARAM wParam, LPARAM lParam ,bool IsReadyPlay); 

	void PlayAni();
	bool Stop();

private:
	CTrayIcon(); 
	CTrayIcon( HWND hWnd, WORD uCallbackMessage, LPCTSTR szTip, HICON icon, WORD uID);

protected: 

	NOTIFYICONDATA m_tnd;       // 数据结构，请参考在线帮助 
	CPoint m_trayCursorPos;     // 托盘响应WM_MOUSEMOVE时的鼠标坐标

	DWORD m_wTrayIconID;
	CPaintManagerUI *m_pManager;
	int m_nCurrFrame;

	HWND m_hWndTrayRMenu;
};

extern HINSTANCE g_hPcmtInstance;