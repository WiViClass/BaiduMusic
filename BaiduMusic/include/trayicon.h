#pragma once
#include "duilib.h"


class CTrayIcon
{ 
public: 
	~CTrayIcon(); 
	static CTrayIcon& Instance();

public: 
	// ����ϵͳͼ�� 
	BOOL Create( HWND hWnd,  WORD uCallbackMessage, LPCTSTR szTip, HICON hIcon, WORD uID);

	// ��ȡNOTIFYICONDATA��Ϣ
	NOTIFYICONDATA GetNotifyIconData(){ return m_tnd; }

	// ������ʾ�ı� 
	BOOL SetTooltipText( LPCTSTR pszTooltipText );  
	CDuiString GetTooltipText() const; 

	// ����ͼ�� 
	BOOL SetIcon( HICON hIcon ); 
	BOOL SetIcon( LPCTSTR lpIconName ); 
	BOOL SetIcon( WORD nIDResource ); 
	HICON GetIcon() const; 
	BOOL HideIcon(); 
	BOOL ShowIcon(); 
	BOOL DeleteIcon(); 

	// ����֪ͨ���� 
	bool SetNotificationWnd( HWND NotifyWnd ); 
	HWND GetNotificationWnd() const; 

	CPoint GetTrayCursorPos();

	// �Զ�����Ϣ������ 
	virtual LRESULT OnIconNotification( WPARAM wParam, LPARAM lParam ,bool IsReadyPlay); 

	void PlayAni();
	bool Stop();

private:
	CTrayIcon(); 
	CTrayIcon( HWND hWnd, WORD uCallbackMessage, LPCTSTR szTip, HICON icon, WORD uID);

protected: 

	NOTIFYICONDATA m_tnd;       // ���ݽṹ����ο����߰��� 
	CPoint m_trayCursorPos;     // ������ӦWM_MOUSEMOVEʱ���������

	DWORD m_wTrayIconID;
	CPaintManagerUI *m_pManager;
	int m_nCurrFrame;

	HWND m_hWndTrayRMenu;
};

extern HINSTANCE g_hPcmtInstance;