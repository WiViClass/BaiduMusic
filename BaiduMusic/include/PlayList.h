#pragma once
#include "duilib.h"
class  CPlaylist : public CUIListDefineBase
{
public:
	CPlaylist();
virtual void DoEvent( TEventUI& event );
void SetItemActive( CControlUI* pControl, int iIndex );
void SetItemActive(int iIndex);
int GetActiveItem();

protected:
	//bool m_bScrollSelect;
	int m_iCurActiveItem;

};