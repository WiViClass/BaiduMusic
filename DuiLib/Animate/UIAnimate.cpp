#include "StdAfx.h"
#include "UIAnimate.h"
//该控件适用于播放循环动画，即一张图片，循环显示不同位置，形成的动画效果。

namespace DuiLib
{



CAnimate::CAnimate()
{

}


CAnimate::~CAnimate()
{

}

LPCTSTR CAnimate::GetClass()
{
	return _T("AnimateUI");
}

LPVOID CAnimate::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_ANIMATION) == 0 ) return static_cast<CAnimate*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CAnimate::DoEvent(TEventUI& event)
{

	if( event.Type == UIEVENT_TIMER )
	{
		PlayAnim();
	}
	CControlUI::DoEvent(event);
}


void CAnimate::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("startimage")) == 0 ) SetStartImage(pstrValue);
	else if( _tcscmp(pstrName, _T("endimage")) == 0 ) SetEndImage(pstrValue);
	else if( _tcscmp(pstrName, _T("pauseimage")) == 0 ) SetPauseImage(pstrValue);
	else if( _tcscmp(pstrName, _T("animimage")) == 0 ) SetAnimImage(pstrValue);
	else if( _tcscmp(pstrName, _T("source")) == 0 )
	{
		RECT rcPos = { 0 };
		LPTSTR pstr = NULL;
		rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
		SetSource(rcPos);
	}
	else if( _tcscmp(pstrName, _T("animstart")) == 0 )
	{
		SetAnimStart(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("animstep")) == 0 )
	{
		SetAnimStep(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("animend")) == 0 )
	{
		SetAnimEnd(_ttoi(pstrValue));
	}
	else CControlUI::SetAttribute(pstrName, pstrValue);
}


void CAnimate::PaintText(HDC hDC)
{
}


void CAnimate::DoPaint(HDC hDC, const RECT& rcPaint)
{
	if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) )
	{
		return;
	}
	//// 绘制循序：背景颜色->文本->动态图
	//DrawItemBk(hDC, m_rcItem);
	//DrawItemText(hDC, m_rcItem);
	//DrawButton(hDC,m_rcItem);
	DrawAnimate(hDC,m_rcItem);

}


void CAnimate::DrawAnimate(HDC hDC, const RECT& rcItem)
{

	CRenderEngine::DrawImageAnim(hDC,GetManager(),GetAnimImage(),rcItem, m_rcAnimSource );
	
	//CDuiString sLog = _T("");

	//sLog.Format(_T(" Draw : %d:%d"),rcItem.left, rcItem.right);

	//WriteLog( sLog );
	
	return;
}


void CAnimate::SetStartImage(LPCTSTR pStrImage)
{
	if( m_sStartImage == pStrImage ) return;

	m_sStartImage = pStrImage;
	Invalidate();
}
void CAnimate::SetEndImage(LPCTSTR pStrImage)
{
	if( m_sEndImage == pStrImage ) return;

	m_sEndImage = pStrImage;
	Invalidate();
}
void CAnimate::SetPauseImage(LPCTSTR pStrImage)
{
	if( m_sPauseImage == pStrImage ) return;

	m_sPauseImage = pStrImage;
	Invalidate();
}
void CAnimate::SetAnimImage(LPCTSTR pStrImage)
{
	if( m_sAnimImage == pStrImage ) return;

	m_sAnimImage = pStrImage;
	//Invalidate();
}

void CAnimate::SetSource(RECT rc)
{
	m_rcAnimSource = rc;
	m_rcItemSource = rc;
	Invalidate();
}

void CAnimate::SetAnimStart(int iStart)
{
	if( iStart < 0 ) return; 
	m_animStart = iStart;
}
void CAnimate::SetAnimStep(int iStep)
{
	if( iStep < 0 ) return; 
	m_animStep = iStep;
}
void CAnimate::SetAnimEnd(int iEnd)
{
	if( iEnd < 0 ) return; 
	m_animEnd = iEnd;
}


LPCTSTR CAnimate::GetStartImage()const
{  
	return m_sStartImage  ;     
}
LPCTSTR CAnimate::GetEndImage()const
{  
	return  m_sEndImage   ;    
}
LPCTSTR CAnimate::GetPauseImage()const
{ 
	return  m_sPauseImage;     
}
LPCTSTR CAnimate::GetAnimImage()const
{  
	return  m_sAnimImage;   
}

RECT CAnimate::GetSource()const
{
	return m_rcItemSource;     
}

int CAnimate::GetAnimStart()const
{
	return  m_animStart;     
}
int CAnimate::GetAnimStep()const
{
	return  m_animStep   ;    
}
int CAnimate::GetAnimEnd()const
{
	return  m_animEnd   ;    
}

void CAnimate::PlayAnim()
{
	if(m_animStep)
	{
		if(m_rcAnimSource.right +m_animStep < m_animEnd)
		{
			m_rcAnimSource.left += m_animStep;
			m_rcAnimSource.right += m_animStep;


			//Invalidate();
		//	GetPaintDC
			//DrawAnimate(m_pManager->GetPaintDC(),GetPos());
			//NeedParentUpdate();


			//CDuiString sLog = _T("");

			//sLog.Format(_T(" Anim : %d:%d"),m_rcAnimSource.left, m_rcAnimSource.right);

			//WriteLog( sLog );

		}else
		{
			m_rcAnimSource = GetSource();
		}

	}
}



void CAnimate::AnimSetting(int istart,int istep,int iend)
{
	 m_animStart =  istart;
	 m_animStep =  istep;
	 m_animEnd = iend;
}

void CAnimate::SetAnimSource(RECT rc)
{

	Invalidate();
}


}