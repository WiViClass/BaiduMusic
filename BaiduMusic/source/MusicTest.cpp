

#include "..\include\MusicTest.h"




CMusicTest::CMusicTest()
	: m_pSend(NULL),
	//m_MusicPlayer(this), 
	m_pPlay(NULL),
	m_pMore(NULL),
	m_pMedia(NULL)
	//m_bActive(false)
{

	MediaInfoState[0]=0;
	MediaInfoState[1]=0;
	MediaInfoState[2]=0;
	MediaInfoState[3]=0;




	pHorizTotal = new CHorizontalLayoutUI();
	pHorizPlay = new CHorizontalLayoutUI();

	m_pAnimate = new CAnimate();


	m_pTitle = new CTextUI();
	m_pArtist = new CTextUI();
	m_pAlbum = new CTextUI();
	m_pTime = new CTextUI();


	m_pTitle->SetMouseEnabled(false);

	this->SetFixedHeight(40);
	this->SetFixedWidth(825);

	pHorizPlay->SetFixedWidth(315);
	m_pTitle->SetFixedWidth(2);
	m_pAnimate->SetFixedWidth(5);

	m_pAnimate->SetAttribute(_T("animimage"),MUSIC_ANIMATE_PATH);
	m_pAnimate->SetAttribute(_T("source"),MUSIC_ANIMATE_SOURCE);

	m_pAnimate->AnimSetting(0,14,195);	
	//m_pAnimate->SetBkColor(0xFFE9F5FF);

	m_pAnimate->SetVisible(true);
	pHorizPlay->Add(m_pTitle);
	pHorizPlay->Add(m_pAnimate);
	pHorizPlay->SetFloat(true);


	m_pArtist->SetFixedWidth(204);
	m_pAlbum->SetFixedWidth(204);
	m_pTime->SetFixedWidth(102);


	m_pArtist->SetFloat(true);
	m_pAlbum->SetFloat(true);
	m_pTime->SetFloat(true);


	

	pHorizTotal->Add(pHorizPlay);
	pHorizTotal->Add(m_pArtist);
	pHorizTotal->Add(m_pAlbum);
	pHorizTotal->Add(m_pTime);
	Add(pHorizTotal);

}

CMusicTest::~CMusicTest()
{

	//m_aTexts.Empty();
}

LPCTSTR CMusicTest::GetClass() const
{
	return _T("MusicListElementUI");
}

void CMusicTest::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
		else CListContainerElementUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_DBLCLICK )
	{
		if( IsEnabled() ) {
			Activate();
			Invalidate();
		}
		if( m_pOwner != NULL ) 
		{
			m_pOwner->DoEvent(event); 
		}else
		{
			return;
		}
	}

	


	if( event.Type == UIEVENT_TIMER )
	{

	}

	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) 
		{
			int m_CurColumn = GetColumnIndex(event.ptMouse);
			MediaInfoState[m_CurColumn]  |= UISTATE_HOT;
			//m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		//return;
	}


	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		///int m_CurColumn = GetColumnIndex(event.ptMouse);
		for( int i = 0; i < MEDIA_COLUMN_MAX; i++ )
		{
			MediaInfoState[i]  &= ~UISTATE_HOT;

		}

		Invalidate();
		//return;
	}



	CListContainerElementUI::DoEvent(event);
}

int CMusicTest::GetColumnIndex( POINT cPoint )
{
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	//cPoint
	for( int i = 0; i < pInfo->nColumns; i++ )
	{
		if(cPoint.x <= pInfo->rcColumn[i].right)
			return i;

	}
	return -1;
}


bool CMusicTest::Activate()
{
	if( !CControlUI::Activate() ) return false;
	if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
	return true;
}

void CMusicTest::DrawItemBk(HDC hDC, const RECT& rcItem)
{
	ASSERT(m_pOwner);
	if( m_pOwner == NULL ) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	DWORD iBackColor = 0;
	if( !pInfo->bAlternateBk || m_iIndex % 2 ) 
	{
		iBackColor = pInfo->dwBkColor;
	}

	if( IsSelected() ) {
		iBackColor = pInfo->dwSelectedBkColor;
	}
	if( !IsEnabled() ) {
		iBackColor = pInfo->dwDisabledBkColor;
	}

	if ( iBackColor != 0 ) {
		CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(iBackColor));
	}




	


	if ( pInfo->dwLineColor != 0 ) {
		RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
		CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
	}
}

void CMusicTest::DrawItemText(HDC hDC, const RECT& rcItem)
{
	if( m_pOwner == NULL ) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();
	DWORD iTextColor = pInfo->dwTextColor;

	if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		//iTextColor = pInfo->dwHotTextColor;
	}
	if( IsSelected() ) {
		//	iTextColor = pInfo->dwSelectedTextColor;
	}
	if( !IsEnabled() ) {
		iTextColor = pInfo->dwDisabledTextColor;
	}
	//IListCallbackUI* pCallback = m_pOwner->GetTextCallback();
	//ASSERT(pCallback);
	//if( pCallback == NULL ) return;

	for( int i = 0; i < pInfo->nColumns; i++ )
	{
		if( i && (MediaInfoState[i] & UISTATE_HOT) != 0 ) 
		{
			iTextColor = pInfo->dwHotTextColor;
		}else 
		{
			iTextColor = pInfo->dwTextColor;
		}

		if(i == 0 && m_bActive)
		{
			iTextColor = pInfo->dwHotTextColor;
		}


		RECT rcItem = { pInfo->rcColumn[i].left, m_rcItem.top, pInfo->rcColumn[i].right, m_rcItem.bottom };
		rcItem.left += pInfo->rcTextPadding.left;
		rcItem.right -= pInfo->rcTextPadding.right;
		rcItem.top += pInfo->rcTextPadding.top;
		rcItem.bottom -= pInfo->rcTextPadding.bottom;

		CDuiString strText;//不使用LPCTSTR，否则限制太多 by cddjr 2011/10/20

			strText.Assign(GetText(i));


		CRenderEngine::DrawText(hDC, m_pManager, rcItem, strText.GetData(), iTextColor, \
			pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
	}
}
void CMusicTest::DrawButton(HDC hDC, const RECT& rcItem)
{
	
}
void CMusicTest::DrawAnimate(HDC hDC, const RECT& rcItem)
{

	if( m_pOwner == NULL ) return;
	TListInfoUI* pInfo = m_pOwner->GetListInfo();

	LPCTSTR pStrModify = NULL;
	int m_rcTile = m_pTitle->GetWidth();

	RECT m_rcAnimate = { pInfo->rcColumn[0].left, m_rcItem.top+16, pInfo->rcColumn[0].left, m_rcItem.bottom-16 };


	m_rcAnimate.left += m_rcTile;
	m_rcAnimate.right = m_rcAnimate.left + 14;

	m_pAnimate->DrawAnimate(hDC,m_rcAnimate);

   // CRenderEngine::DrawImageString(hDC, m_pAnimate->GetManager(), m_rcAnimate, m_rcAnimate, m_pAnimate->GetBkImage(), pStrModify);

}

void CMusicTest::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) )
	{
		return;
	}
	 // 绘制循序：背景颜色->文本->动态图
	DrawItemBk(hDC, m_rcItem);
	DrawItemText(hDC, m_rcItem);
	DrawButton(hDC,m_rcItem);
	DrawAnimate(hDC,m_rcItem);

}

void CMusicTest::CElementSetValue(MEDIATEST *m_Media)
{
	//SetItemTextPadding(CDuiRect(22,5,0,5));
	m_pMedia = m_Media;
	//pListElement->SetTextPadding(CDuiRect(2, 0, 2, 0));

	//SetText(0, m_pMedia->Title);
	//SetText(1, m_pMedia->Artist);
	//SetText(2, m_pMedia->Album);
	//SetText(3, m_pMedia->StringTime);


}
MEDIATEST*  CMusicTest::GetMedia()
{
	return m_pMedia;
}

void  CMusicTest::SetActive(bool bActive)
{

	if( !IsEnabled() ) 
		return ;
	if( bActive == m_bActive ) 
		return ;
	m_bActive = bActive;
	Invalidate();

}
bool  CMusicTest::GetActive()
{
	return m_bActive;
}

void CMusicTest::SetText(int iIndex, LPCTSTR pstrText)
{
	switch (iIndex)
	{
		case 0:
		{
			m_pTitle->SetText(pstrText);

		}
		break;

		case 1:
			{
				m_pArtist->SetText(pstrText);

			}
		break;

		case 2:
			{
				m_pAlbum->SetText(pstrText);
				m_pAlbum->SetBkColor(0xFFE9F5FF);
			}
		break;

		case 3:
			{
				m_pTime->SetText(pstrText);
			}
		break;


		default:
		break;
	}


}

LPCTSTR CMusicTest::GetText(int iIndex) const
{
	switch (iIndex)
	{
	case 0:
		{
			 return m_pTitle->GetText();

		}
		break;

	case 1:
		{
			return m_pArtist->GetText();
		}
		break;

	case 2:
		{
			 return m_pAlbum->GetText();
		}
		break;

	case 3:
		{
			 return m_pTime->GetText();
		}
		break;


	default:
		break;
	}
	return NULL;
}


#if 0
void CMusicListElementUI::SetOwner(CControlUI* pOwner)
{
	// CListContainerElementUI::SetOwner(pOwner);
	m_pListOwner = static_cast<IListUI*>(pOwner->GetInterface(_T("IList")));
}

LPCTSTR CMusicListElementUI::GetText(int iIndex) const
{
	CDuiString* pText = static_cast<CDuiString*>(m_aTexts.GetAt(iIndex));
	if( pText ) return pText->GetData();
	return NULL;
}

void CMusicListElementUI::SetText(int iIndex, LPCTSTR pstrText)
{
	if( m_pListOwner == NULL ) return;
	TListInfoUI* pInfo = m_pListOwner->GetListInfo();
	if( iIndex < 0 || iIndex >= pInfo->nColumns ) return;
	while( m_aTexts.GetSize() < pInfo->nColumns ) { m_aTexts.Add(NULL); }

	CDuiString* pText = static_cast<CDuiString*>(m_aTexts[iIndex]);
	if( (pText == NULL && pstrText == NULL) || (pText && *pText == pstrText) ) return;

	if ( pText ) //by cddjr 2011/10/20
		pText->Assign(pstrText);
	else
		m_aTexts.SetAt(iIndex, new CDuiString(pstrText));
	Invalidate();
}

void CMusicListElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
{
	if( m_pListOwner == NULL ) return;
	TListInfoUI* pInfo = m_pListOwner->GetListInfo();
	DWORD iTextColor = pInfo->dwTextColor;

	if( (m_uButtonState & UISTATE_HOT) != 0 ) {
		iTextColor = pInfo->dwHotTextColor;
	}
	if( IsSelected() ) {
		iTextColor = pInfo->dwSelectedTextColor;
	}
	if( !IsEnabled() ) {
		iTextColor = pInfo->dwDisabledTextColor;
	}
	IListCallbackUI* pCallback = m_pListOwner->GetTextCallback();
	//ASSERT(pCallback);
	//if( pCallback == NULL ) return;

	//m_nLinks = 0;
	//int nLinks = lengthof(m_rcLinks);
	for( int i = 0; i < pInfo->nColumns; i++ )
	{
		RECT rcItem = { pInfo->rcColumn[i].left, m_rcItem.top, pInfo->rcColumn[i].right, m_rcItem.bottom };
		rcItem.left += pInfo->rcTextPadding.left;
		rcItem.right -= pInfo->rcTextPadding.right;
		rcItem.top += pInfo->rcTextPadding.top;
		rcItem.bottom -= pInfo->rcTextPadding.bottom;

		CDuiString strText;//不使用LPCTSTR，否则限制太多 by cddjr 2011/10/20
		if( pCallback ) strText = pCallback->GetItemText(this, m_iIndex, i);
		else strText.Assign(GetText(i));

		CRenderEngine::DrawText(hDC, m_pManager, rcItem, strText.GetData(), iTextColor, \
			pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);

		//m_nLinks += nLinks;
		//nLinks = lengthof(m_rcLinks) - m_nLinks; 
	}

}
#endif