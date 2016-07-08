#include "..\include\UIAlbum.h"

LPCTSTR CAlbumUI::GetClass() const
{
	return _T("MyAlbum");
}

LPVOID CAlbumUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_ALBUM) == 0 ) return static_cast<CAlbumUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void  CAlbumUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	RECT rcBmpPart = {0};
	RECT rcCorner = {0};
	DWORD dwMask = 0;
	//DWORD dwMask = 0;

	BYTE bFade = 0xFF;
	bool bHole = false;
	bool bTiledX = false;
	bool bTiledY = false;
	bool balphaChannel = false;
	m_rcPaint = m_rcItem;



	HDC hCloneDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, m_AlbumPic.hBitmap);
	::SetStretchBltMode(hDC, HALFTONE);
	//if()
	::BitBlt(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, \
		hCloneDC, 0, 0, SRCCOPY);

	//CRenderEngine::DrawImage(hDC, hBmpAlbum, m_rcItem, m_rcPaint, rcBmpPart, rcCorner, balphaChannel, bFade, bHole, bTiledX, bTiledY);

	//return true;
}
void  CAlbumUI::SetHBitMap(HBITMAP hBmp)
{
	hBmpAlbum = hBmp;
}
HBITMAP  CAlbumUI::GetHBitMap()
{
	return hBmpAlbum;
}

void  CAlbumUI::SetPicture(TID3Picture mPic)
{
	m_AlbumPic = mPic;
}
TID3Picture  CAlbumUI::GetPicture()
{
	return m_AlbumPic;
}
