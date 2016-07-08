#include "StdAfx.h"
#include "UIListMult.h"

namespace DuiLib
{

CUIListDefineBase::CUIListDefineBase()
:  delay_deltaY_( 0 )
, delay_number_( 0 )
, delay_left_( 0 )
, level_text_start_pos_( 7 )
, text_padding_( 13, 0, 0, 0 )
{
	//SetItemShowHtml( true );


}

CUIListDefineBase::~CUIListDefineBase()
{

}

bool CUIListDefineBase::Add( CControlUI* pControl )
{
	if ( !pControl )
	{
		return false;
	}

	if ( _tcsicmp( pControl->GetClass(), _T("ListContainerElementUI") ) != 0 )
	{
	//	return false;
	}

	return CListUI::Add( pControl );
}

bool CUIListDefineBase::AddAt( CControlUI* pControl, int iIndex )
{
	if ( !pControl )
	{
		return false;
	}

	if ( _tcsicmp( pControl->GetClass(), _T("ListContainerElementUI") ) != 0 )
	{
		return false;
	}

	return CListUI::AddAt( pControl, iIndex );
}

bool CUIListDefineBase::Remove( CControlUI* pControl )
{
	if ( !pControl )
	{
		return false;
	}

	if ( _tcsicmp( pControl->GetClass(), _T("ListContainerElementUI") ) != 0 )
	{
		return false;
	}

	// ��ѡ���б�����ɾ����Ӧ��
	std::set<int>::iterator itor = m_setSelectItems.find( GetItemIndex( pControl ) );
	if( itor != m_setSelectItems.end() )
	{
		m_setSelectItems.erase( itor );
	}


		return CListUI::Remove( pControl );

}

bool CUIListDefineBase::RemoveAt( int iIndex )
{
	CControlUI* pControl = GetItemAt( iIndex );
	if ( !pControl )
	{
		return false;
	}

	if ( _tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0 )
	{
		return false;
	}

	// ��ѡ���б�����ɾ����Ӧ��
	std::set<int>::iterator itor = m_setSelectItems.find( iIndex );
	if( itor != m_setSelectItems.end() )
	{
		m_setSelectItems.erase( itor );
	}


		return CListUI::RemoveAt( iIndex );

}

void CUIListDefineBase::RemoveAll()
{
	m_setSelectItems.clear();

	CListUI::RemoveAll();


}

void CUIListDefineBase::DoEvent( TEventUI& event ) 
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
	if(event.Type == UIEVENT_BUTTONUP)
	{
		if(m_bMouseSelect)
		{
			m_bMouseSelect = false ;
		}
	}

	if(event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT_MOUSEHOVER && m_pList->m_bVScrollBarShow)
	{
		GetItemPos(event.ptMouse);

	}

	if(event.Type == UIEVENT_ITEMMOUSEMOVE)
	{
		if(GetCount() < 0)
		{
			return ;
		}
		if(!m_bMouseSelect)
		{
			m_bMouseSelect = true ;
			UnSelectAll();
		}

		int selectTmp = GetCurItem(event.ptMouse); 
		if(selectTmp >=0)
		{
			MouseSelect(selectTmp);
		}
		return ;
	}


	switch( event.Type ) 
	{
	case UIEVENT_KEYDOWN:
		switch( event.chKey ) 
		{
		case 0x41://�ػ����Ctrl+A
			if( GetKeyState( VK_CONTROL ) < 0 )
			{
				SelectAll();
				return ;
			}
		}
		break;
	case UIEVENT_SCROLLWHEEL:
		{

		}
		break;
	}


	CListUI::DoEvent( event );
}

bool CMultiSelListUI::IsMultiSelectItem()const
{
	return m_bMultiSelect;
}

bool CMultiSelListUI::IsBeginSelectItem()
{
	return m_bBeginSelect;
}

void CMultiSelListUI::SetMultiSelectItem( bool bMultiSel )
{
	m_bMultiSelect = bMultiSel;
}

void CMultiSelListUI::SetBeginSelectItem( bool bBeginSel /*= true*/ )
{
	m_bBeginSelect = bBeginSel;
}

std::set<int> CMultiSelListUI::GetSelectItems()
{
	if ( !IsMultiSelectItem() )
	{
		m_setSelectItems.clear();
		if ( m_iCurSel != -1 )
		{
			m_setSelectItems.insert( m_iCurSel );
		}		
	}
	return m_setSelectItems;
}

bool CMultiSelListUI::SelectItem( int iIndex, bool bTakeFocus )
{
	if( iIndex < 0 || iIndex >= GetCount() ) return false;
	//��껬��ѡ��ʱ��MouseSelect�в���

	CDuiString sLog = _T("");

	sLog.Format(_T("SelectItem : %d:%d:%d"),iIndex, GetKeyState(VK_CONTROL),iIndex);

	WriteLog( sLog );


	// CTRL��SHIFTͬʱ���·���
	if ( GetKeyState(VK_CONTROL) < 0 && GetKeyState(VK_SHIFT) < 0 )
	{
		return true;
	}

	if ( !IsMultiSelectItem() )
	{
		return __super::SelectItem( iIndex, bTakeFocus );
	}

	//�˷�֧�����������
	//1����֧�ֶ�ѡ���
	//2����ǰ�����������ʱ��ѡ�е������ϵ��
	//3��ѡ���б��и�������1ʱ������һ����������������˵�
	if ( GetKeyState(VK_CONTROL) >= 0 && GetKeyState(VK_SHIFT) >= 0 )
	{
		if ( GetKeyState( VK_RBUTTON ) < 0 )
		{
			// �����ѡ������Ҽ������ѡ��ĳ���˻��ǵ����˵�
			if ( m_setSelectItems.size() > 1 )
			{
				if ( m_setSelectItems.find( iIndex ) != m_setSelectItems.end() )
				{
					return true;
				}
			}

			// ���û�а��µ�ѡ��item��ô��Ҫ�������ѡ����浱ǰѡ�
			for ( std::set<int>::iterator it = m_setSelectItems.begin(); it != m_setSelectItems.end(); it++)
			{
				CControlUI* pElement = GetItemAt(*it);
				if ( pElement )
				{
					IListItemUI *pIListItem = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
					if ( pIListItem && pIListItem->GetIndex() != iIndex )
					{
						pIListItem->Select( false );
					}
				}
			}
			m_setSelectItems.clear();
			if ( iIndex != -1 )
			{
				m_setSelectItems.insert( iIndex );
			}
			return __super::SelectItem( iIndex, bTakeFocus );
		}

		else //if ( GetKeyState( VK_LBUTTON ) < 0 )
		{
			// ���û�а��µ�ѡ��item��ô��Ҫ�������ѡ����浱ǰѡ�
			for ( std::set<int>::iterator it = m_setSelectItems.begin(); it != m_setSelectItems.end(); it++)
			{
				CControlUI* pElement = GetItemAt(*it);
				if ( pElement )
				{
					IListItemUI *pIListItem = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
					if ( pIListItem && iIndex != *it )
					{
						pIListItem->Select( false );
					}
				}
			}
			m_setSelectItems.clear();
			if ( iIndex != -1 )
			{
				m_setSelectItems.insert( iIndex );
			}
			return CListUI::SelectItem( iIndex, bTakeFocus );
		}
	} 

	if ( GetKeyState(VK_CONTROL) < 0 ) // ��סctrl
	{
		for ( std::set<int>::iterator it = m_setSelectItems.begin(); it != m_setSelectItems.end(); it++)
		{
			if ( *it == iIndex )
			{
				CControlUI* pControl = GetItemAt( iIndex );
				if( pControl != NULL) 
				{
					IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
					if( pListItem != NULL )
					{
						pListItem->Select( false );
						it = m_setSelectItems.erase( it );
						m_pManager->SendNotify( this, DUI_MSGTYPE_ITEMSELECT, iIndex, m_iCurSel );
						return true;
					}
				}
			}
		}

		if ( iIndex != -1 )
		{
			m_setSelectItems.insert( iIndex );
		}
		
		m_pManager->SendNotify( this, DUI_MSGTYPE_ITEMSELECT, iIndex, m_iCurSel );
		
		return 0;
	}
	else if ( GetKeyState(VK_SHIFT) < 0 ) // ��סshift
	{
		if ( m_bBeginSelect ) //���ε��ڶ��ε�ѡ���¼�
		{
			m_pManager->SendNotify( this, DUI_MSGTYPE_ITEMSELECT, iIndex, m_iCurSel );
			return true;
		}

		if ( m_iCurSel == -1 ) //֮ǰû��ѡ���κ�
		{
			m_iCurSel = iIndex; // ��Ϊ��ʼ��
		}
		else
		{
			m_bBeginSelect = true;
			int iStartSel = min(m_iCurSel, iIndex);
			int iEndSel = max(m_iCurSel, iIndex);
			//֮ǰ��ѡ����ȫ����գ�����ѡ��
			for ( std::set<int>::iterator it = m_setSelectItems.begin(); it != m_setSelectItems.end(); it++)
			{
				CControlUI* pElement = GetItemAt(*it);
				if ( pElement )
				{
					IListItemUI *pList = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
					if ( pList )
					{
						pList->Select(false);
					}		
				}
			}
			m_setSelectItems.clear();

			for ( ; iStartSel <= iEndSel; iStartSel++ )
			{
				if (iStartSel == -1)
				{
					continue;
				}
				IListItemUI *pList = static_cast<IListItemUI*>(GetItemAt(iStartSel)->GetInterface(_T("ListItem")));
				if (pList)
				{
					pList->Select(true);
				}
				m_setSelectItems.insert( iStartSel );
			}
			m_bBeginSelect = false;
			m_pManager->SendNotify( this, DUI_MSGTYPE_ITEMSELECT, iIndex, m_iCurSel );
			return true;
		}
	}

    return __super::SelectItem( iIndex, bTakeFocus );
}

CMultiSelListUI::CMultiSelListUI()
: CListUI()
, m_bMultiSelect( true )
, m_bBeginSelect( false )
{
	m_setSelectItems.clear();
}

CMultiSelListUI::~CMultiSelListUI()
{

}

LPCTSTR CMultiSelListUI::GetClass() const
{
	return _T("MultiSelList");
}

LPVOID CMultiSelListUI::GetInterface( LPCTSTR pstrName )
{
	return CListUI::GetInterface( pstrName );
}

int CMultiSelListUI::GetCurSel()
{
	if ( IsMultiSelectItem() )
	{
		ASSERT( NULL && _T("should not be called while m_bMultiSelect is true") );
		return -1;
	}
	else
	{
		return CListUI::GetCurSel();
	}
}

void CMultiSelListUI::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if( _tcscmp(pstrName, _T("multiselect")) == 0 )
	{
		SetMultiSelectItem(_tcscmp(pstrValue, _T("true")) == 0);
		return;
	}	
	__super::SetAttribute( pstrName, pstrValue );
}

void CMultiSelListUI::UnSelectAll( )
{
	// ���û�а��µ�ѡ��item��ô��Ҫ�������ѡ����浱ǰѡ�
	for ( std::set<int>::iterator it = m_setSelectItems.begin(); it != m_setSelectItems.end(); it++)
	{
		CControlUI* pElement = GetItemAt(*it);
		if ( pElement )
		{
			IListItemUI *pIListItem = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
			if ( pIListItem )
			{
				pIListItem->Select( false );
			}
		}
	}
	m_setSelectItems.clear();
	m_itemStart = -1;
	m_itemEnd = -1;
}
void CMultiSelListUI::SelectAll()
{
	//����б�Ϊ���򷵻�

	int itemCount = GetCount();

	if(itemCount<0)
	{
		return ;
	}

	m_setSelectItems.clear();
	for ( int itemIndex = 0; itemIndex <=itemCount; itemIndex++)
	{
		CControlUI* pElement = GetItemAt(itemIndex);
		if ( pElement )
		{
			IListItemUI *pIListItem = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
			if ( pIListItem )
			{
				pIListItem->Select( true );
			}
		}
		m_setSelectItems.insert(itemIndex);
	}
	
}

int  CMultiSelListUI::GetCurItem(POINT pt )
{
	//int itemIndex;
	CControlUI* pNewMove = m_pManager->FindControl(pt);
	if(pNewMove == NULL)
	{
		return -1;
	}

	CDuiString sLog = pNewMove->GetParent()->GetClass();

	//sLog.Format(_T("SelectItem : %d:%d:%d"),m_itemStart, m_itemEnd,iIndex);

	WriteLog( sLog );


	CDuiString CControlTmp = pNewMove->GetClass();

	if(_tcsstr(pNewMove->GetClass(), _T("MultiSelList")) != NULL )
	{
		return GetCount();

	}
	else 
	{
		int itemTemp = GetItemIndex(pNewMove);

		if(itemTemp == 0xFFFFFFFF  && pNewMove->GetParent() != NULL)
			itemTemp = GetItemIndex(pNewMove->GetParent());
		return itemTemp;

	}

}

bool CMultiSelListUI::MouseSelect(int iIndex )
{

	if(m_itemEnd == iIndex )
	{
		return true;
	}
	CDuiString sLog = _T("");

	sLog.Format(_T("SelectItem : %d:%d:%d"),m_itemStart, m_itemEnd,iIndex);

	WriteLog( sLog );


	if(m_setSelectItems.empty())
	{
		SetSelect(iIndex,true );
		m_setSelectItems.insert(iIndex);
		m_itemStart = iIndex;
		m_itemEnd = iIndex;
		return true;
	}

	if(iIndex > m_itemEnd)
	{
		if(m_itemEnd >= m_itemStart )
		{
			SetSelect(iIndex,true );
			m_setSelectItems.insert(iIndex);
			
		}
		else
		{
			SetSelect(m_itemEnd,false );
			m_setSelectItems.erase(m_itemEnd);
			
		}

	}else
	{
		if(m_itemEnd <= m_itemStart )
		{
			SetSelect(iIndex,true );
			m_setSelectItems.insert(iIndex);
		}
		else
		{
			SetSelect(m_itemEnd,false );
			m_setSelectItems.erase(m_itemEnd);
		
		}
	}
	m_itemEnd = iIndex;
	return true;
}

bool CMultiSelListUI::SetSelect(int iIndex ,bool bSelect)
{


	if( iIndex < 0 || iIndex >= GetCount() ) return false;
	CControlUI* pElement = GetItemAt(iIndex);
	if(pElement)
	{
		IListItemUI *pIListItem = static_cast<IListItemUI*>(pElement->GetInterface(_T("ListItem")));
		if ( pIListItem && pIListItem->GetIndex() == iIndex )
		{
			pIListItem->SetSelect(bSelect);
		}
	}
	return true;
}


} // namespace DuiLib