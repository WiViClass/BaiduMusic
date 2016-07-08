#pragma once
#include "duilib.h"

using namespace libZPlay;

#define MEDIA_COLUMN_MAX 4

typedef struct tagMEDIA
{
	CDuiString  name;              //����
	CDuiString   pathFileName;      //�ļ�·����
	CDuiString  logoPath;		   //logoͼƬ��·��
	CDuiString	exName;			   //��չ��
	CDuiString  Title;
	CDuiString  Artist;
	CDuiString  Album;              //����
	CDuiString  StringTime;         //�ܲ���ʱ��CDuiString��ʽ
	
	CDuiString  PlayTitle;         //����ʱ��ʾ�ĸ�����Ϣ������ - ���֣�

	//UINT		playNum;		   //���Ŵ���
	//CDuiString	size;			   //��С  (M)
	TStreamTime totalTime;         //��ʱ��	

	//CMusicListItemUI* pControl;    //�󶨵Ŀؼ�ָ��
//	CDuiString  tag;			   //�����ֶΣ����Դ�Ŷ�����Ϣ
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

	CButtonUI*			m_pPlay;//���Ű�ť
	CButtonUI*			m_pSend;//���͵��豸
	CButtonUI*			m_pMore;//����
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
