#if 0


namespace DuiLib {



	////////////////////////////////////////////////////////////////////////////////////
	CListContainerElementUI::CListContainerElementUI() : 
	m_iIndex(-1),
		m_pOwner(NULL), 
		m_bSelected(false),
		m_uButtonState(0)
	{
	}

	LPCTSTR CListContainerElementUI::GetClass() const
	{
		return _T("ListContainerElementUI");
	}

	UINT CListContainerElementUI::GetControlFlags() const
	{
		return UIFLAG_WANTRETURN;
	}

	LPVOID CListContainerElementUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_LISTITEM) == 0 ) return static_cast<IListItemUI*>(this);
		if( _tcscmp(pstrName, DUI_CTR_LISTCONTAINERELEMENT) == 0 ) return static_cast<CListContainerElementUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}

	IListOwnerUI* CListContainerElementUI::GetOwner()
	{
		return m_pOwner;
	}

	void CListContainerElementUI::SetOwner(CControlUI* pOwner)
	{
		m_pOwner = static_cast<IListOwnerUI*>(pOwner->GetInterface(_T("IListOwner")));
	}

	void CListContainerElementUI::SetVisible(bool bVisible)
	{
		CContainerUI::SetVisible(bVisible);
		if( !IsVisible() && m_bSelected)
		{
			m_bSelected = false;
			if( m_pOwner != NULL ) m_pOwner->SelectItem(-1);
		}
	}

	void CListContainerElementUI::SetEnabled(bool bEnable)
	{
		CControlUI::SetEnabled(bEnable);
		if( !IsEnabled() ) {
			m_uButtonState = 0;
		}
	}

	int CListContainerElementUI::GetIndex() const
	{
		return m_iIndex;
	}

	void CListContainerElementUI::SetIndex(int iIndex)
	{
		m_iIndex = iIndex;
	}

	void CListContainerElementUI::Invalidate()
	{
		if( !IsVisible() ) return;

		if( GetParent() ) {
			CContainerUI* pParentContainer = static_cast<CContainerUI*>(GetParent()->GetInterface(_T("Container")));
			if( pParentContainer ) {
				RECT rc = pParentContainer->GetPos();
				RECT rcInset = pParentContainer->GetInset();
				rc.left += rcInset.left;
				rc.top += rcInset.top;
				rc.right -= rcInset.right;
				rc.bottom -= rcInset.bottom;
				CScrollBarUI* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
				if( pVerticalScrollBar && pVerticalScrollBar->IsVisible() ) rc.right -= pVerticalScrollBar->GetFixedWidth();
				CScrollBarUI* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
				if( pHorizontalScrollBar && pHorizontalScrollBar->IsVisible() ) rc.bottom -= pHorizontalScrollBar->GetFixedHeight();

				RECT invalidateRc = m_rcItem;
				if( !::IntersectRect(&invalidateRc, &m_rcItem, &rc) ) 
				{
					return;
				}

				CControlUI* pParent = GetParent();
				RECT rcTemp;
				RECT rcParent;
				while( pParent = pParent->GetParent() )
				{
					rcTemp = invalidateRc;
					rcParent = pParent->GetPos();
					if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
					{
						return;
					}
				}

				if( m_pManager != NULL ) m_pManager->Invalidate(invalidateRc);
			}
			else {
				CContainerUI::Invalidate();
			}
		}
		else {
			CContainerUI::Invalidate();
		}
	}

	bool CListContainerElementUI::Activate()
	{
		if( !CContainerUI::Activate() ) return false;
		if( m_pManager != NULL ) m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
		return true;
	}

	bool CListContainerElementUI::IsSelected() const
	{
		return m_bSelected;
	}

	bool CListContainerElementUI::Select(bool bSelect)
	{
		if( !IsEnabled() ) return false;
		if( bSelect == m_bSelected ) return true;
		m_bSelected = bSelect;
		if( bSelect && m_pOwner != NULL ) m_pOwner->SelectItem(m_iIndex);
		Invalidate();

		return true;
	}

	bool CListContainerElementUI::IsExpanded() const
	{
		return false;
	}

	bool CListContainerElementUI::Expand(bool /*bExpand = true*/)
	{
		return false;
	}

	void CListContainerElementUI::DoEvent(TEventUI& event)
	{
		if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
			if( m_pOwner != NULL ) m_pOwner->DoEvent(event);
			else CContainerUI::DoEvent(event);
			return;
		}

		if( event.Type == UIEVENT_DBLCLICK )
		{
			if( IsEnabled() ) {
				Activate();
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_KEYDOWN && IsEnabled() )
		{
			if( event.chKey == VK_RETURN ) {
				Activate();
				Invalidate();
				return;
			}
		}
		if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_RBUTTONDOWN )
		{
			if( IsEnabled() ){
				m_pManager->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
				Select();
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_BUTTONUP ) 
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEMOVE )
		{
			return;
		}
		if( event.Type == UIEVENT_MOUSEENTER )
		{
			if( IsEnabled() ) {
				m_uButtonState |= UISTATE_HOT;
				Invalidate();
			}
			return;
		}
		if( event.Type == UIEVENT_MOUSELEAVE )
		{
			if( (m_uButtonState & UISTATE_HOT) != 0 ) {
				m_uButtonState &= ~UISTATE_HOT;
				Invalidate();
			}
			return;
		}

		// An important twist: The list-item will send the event not to its immediate
		// parent but to the "attached" list. A list may actually embed several components
		// in its path to the item, but key-presses etc. needs to go to the actual list.
		if( m_pOwner != NULL ) m_pOwner->DoEvent(event); else CControlUI::DoEvent(event);
	}

	void CListContainerElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcscmp(pstrName, _T("selected")) == 0 ) Select();
		else CContainerUI::SetAttribute(pstrName, pstrValue);
	}

	void CListContainerElementUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;
		DrawItemBk(hDC, m_rcItem);
		CContainerUI::DoPaint(hDC, rcPaint);
	}

	void CListContainerElementUI::DrawItemText(HDC hDC, const RECT& rcItem)
	{
		return;
	}

	void CListContainerElementUI::DrawItemBk(HDC hDC, const RECT& rcItem)
	{
		ASSERT(m_pOwner);
		if( m_pOwner == NULL ) return;
		TListInfoUI* pInfo = m_pOwner->GetListInfo();
		DWORD iBackColor = 0;
		if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) 
		{
			iBackColor = pInfo->dwBkColor;
		}

		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			iBackColor = pInfo->dwHotBkColor;
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

		if( !IsEnabled() ) {
			if( !pInfo->sDisabledImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)pInfo->sDisabledImage) ) pInfo->sDisabledImage.Empty();
				else return;
			}
		}
		if( IsSelected() ) {
			if( !pInfo->sSelectedImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)pInfo->sSelectedImage) ) pInfo->sSelectedImage.Empty();
				else return;
			}
		}
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			if( !pInfo->sHotImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)pInfo->sHotImage) ) pInfo->sHotImage.Empty();
				else return;
			}
		}
		if( !m_sBkImage.IsEmpty() ) {
			if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) {
				if( !DrawImage(hDC, (LPCTSTR)m_sBkImage) ) m_sBkImage.Empty();
			}
		}

		if( m_sBkImage.IsEmpty() ) {
			if( !pInfo->sBkImage.IsEmpty() ) {
				if( !DrawImage(hDC, (LPCTSTR)pInfo->sBkImage) ) pInfo->sBkImage.Empty();
				else return;
			}
		}

		if ( pInfo->dwLineColor != 0 ) {
			RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
			CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
		}
	}

} // namespace DuiLib
#endif