#pragma once
#include "duilib.h"

using namespace libZPlay;

#define  DUI_CTR_ALBUM                          (_T("MyAlbum"))
class CAlbumUI: public CControlUI
{
public :
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
	    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
		void SetHBitMap(HBITMAP hBmp);
		HBITMAP GetHBitMap();

		void SetPicture(TID3Picture mPic);
		TID3Picture GetPicture();

protected:

	 HBITMAP hBmpAlbum;
	 TID3Picture  m_AlbumPic;

};

