#include "..\include\PlayList.h"
#include "..\include\MusicListElement.h"



 CPlaylist::CPlaylist()

{

}

 void CPlaylist::DoEvent( TEventUI& event ) 
 {
	 if ( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND )
	 {
		 if ( m_pParent != NULL )
		 {
			 m_pParent->DoEvent( event );
		 }
		 else
		 {
			 CVerticalLayoutUI::DoEvent( event );
		 }
		 return;
	 }

	 if( event.Type == UIEVENT_DBLCLICK )
	 {
		// CListContainerElementUI *m_pSongtmp = static_cast<CListContainerElementUI*>(event.pSender);
		//m_pSongtmp->GetIndex();
		// event.pSender
		 	//	static_cast<CMusicListElementUI*>(event.pSender)->GetIndex();
		// SetItemActive(event.pSender ,indexTmp);
		return;
		
	 }


	 CUIListDefineBase::DoEvent( event );
 }

 void CPlaylist::SetItemActive(CControlUI* pControl ,int iIndex)
 {
	 //if()
	// m_iCurActiveItem = iIndex;

 }


void CPlaylist::SetItemActive(int iIndex)
{
	//if()

	if( iIndex == m_iCurActiveItem ) return ;

	int iOldSel = m_iCurSel;
	// We should first unselect the currently selected item
	if( m_iCurActiveItem >= 0 ) {
		CControlUI* pControl = GetItemAt(m_iCurActiveItem);
		if( pControl != NULL) {
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) pListItem->SetActive(false);
		}

		m_iCurActiveItem = -1;
	}
	

	if( iIndex >= 0 ) {
		CControlUI* pControl = GetItemAt(iIndex);
		if( pControl != NULL) {
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if( pListItem != NULL ) pListItem->SetActive(true);
		}

		//m_iCurActiveItem = -1;
	}

	m_iCurActiveItem = iIndex;
}
int CPlaylist::GetActiveItem()
{
	return m_iCurActiveItem ;
}