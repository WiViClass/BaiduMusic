#pragma once
#include "duilib.h"

using namespace libZPlay;

#define MEDIA_COLUMN_MAX 4

typedef struct tagMEDIA
{
	CDuiString  name;              //名称
	CDuiString   pathFileName;      //文件路径名
	CDuiString  logoPath;		   //logo图片的路径
	CDuiString	exName;			   //扩展名
	CDuiString  Title;
	CDuiString  Artist;
	CDuiString  Album;              //名称
	CDuiString  StringTime;         //总播放时间CDuiString格式
	
	CDuiString  PlayTitle;         //播放时显示的歌曲信息（歌名 - 歌手）

	//UINT		playNum;		   //播放次数
	//CDuiString	size;			   //大小  (M)
	TStreamTime totalTime;         //总时间	

	//CMusicListItemUI* pControl;    //绑定的控件指针
//	CDuiString  tag;			   //保留字段，可以存放额外信息
	//tagMEDIA():pControl(NULL)
	//{
	//	ZeroMemory(&totalTime, sizeof(totalTime));
	//}
}MEDIA;

//using namespace DuiLib;
class  CMusicListElementUI : public CListContainerElementUI
{

public:

	CMusicListElementUI();
	~CMusicListElementUI();
	LPCTSTR GetClass() const;
	void DoEvent(TEventUI& event);
	int GetColumnIndex( POINT cPoint );
	void DrawItemBk(HDC hDC, const RECT& rcItem);
	void DrawItemText(HDC hDC, const RECT& rcItem);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	bool Activate();
	void CElementSetValue(MEDIA *m_Media);
    MEDIA* GetMedia();
   

	void  SetActive(bool bActive);
	bool  GetActive();



	LPCTSTR GetText(int iIndex) const;
	void SetText(int iIndex, LPCTSTR pstrText);
	void DrawButton(HDC hDC, const RECT& rcItem);
	void DrawAnimate(HDC hDC, const RECT& rcItem);
	void AnimatePlay(bool bPlay); 


private:

	CButtonUI*			m_pPlay;//播放按钮
	CButtonUI*			m_pSend;//发送到设备
	CButtonUI*			m_pMore;//更多
	MEDIA*              m_pMedia;
	//bool				m_bActive;
	UINT MediaInfoState[MEDIA_COLUMN_MAX] ;



	//CMusicTest*			m_pList;
	CHorizontalLayoutUI*	pHorizTotal;

	CHorizontalLayoutUI*	pHorizPlay;

	CAnimate*				m_pAnimate;

	CTextUI*			m_pTitle;
	CTextUI*			m_pArtist;
	CTextUI*			m_pAlbum;
	CTextUI*			m_pTime;

	bool				m_bPlayAnim;

};
