#ifndef __UIRENDER_H__
#define __UIRENDER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CRenderClip
{
public:
    ~CRenderClip();
    RECT rcItem;
    HDC hDC;
    HRGN hRgn;
    HRGN hOldRgn;

    static void GenerateClip(HDC hDC, RECT rc, CRenderClip& clip);
    static void GenerateRoundClip(HDC hDC, RECT rc, RECT rcItem, int width, int height, CRenderClip& clip);
    static void UseOldClipBegin(HDC hDC, CRenderClip& clip);
    static void UseOldClipEnd(HDC hDC, CRenderClip& clip);
};

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CRenderEngine
{
public:
    static DWORD AdjustColor(DWORD dwColor, short H, short S, short L);
  //����ͼƬ
	static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    static void FreeImage(const TImageInfo* bitmap);
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
	
	//�Զ������ڶ�������
	static bool DrawImageAnim(HDC hDC,CPaintManagerUI* pManager,LPCTSTR pStrImage,const RECT& rc,const RECT& rcBmpPart);


    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

    // ���º����е���ɫ����alphaֵ��Ч
    static void DrawLine(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor,int nStyle = PS_SOLID);
    static void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
    static void DrawRoundRect(HDC hDC, const RECT& rc, int width, int height, int nSize, DWORD dwPenColor);
    static void DrawText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, \
        DWORD dwTextColor, int iFont, UINT uStyle);
    static void DrawHtmlText(HDC hDC, CPaintManagerUI* pManager, RECT& rc, LPCTSTR pstrText, 
        DWORD dwTextColor, RECT* pLinks, CDuiString* sLinks, int& nLinkRects, UINT uStyle);
    static HBITMAP GenerateBitmap(CPaintManagerUI* pManager, CControlUI* pControl, RECT rc);
	static SIZE GetTextSize(HDC hDC, CPaintManagerUI* pManager , LPCTSTR pstrText, int iFont, UINT uStyle);
};

} // namespace DuiLib

#endif // __UIRENDER_H__
//6 class CRenderEngine  
//	* ��Ⱦ����  
//	* ���� -  
//	LoadImage()  
//	���� - ����ͼ��  
//	bitmap - ͼ��  
//	type - ����  
//	mask - ����ɫ  
//	ʵ�� -  
//	1 ��ȡ�ļ�����Դ��zip����ͼ���ļ����ݵ��ڴ�[ͨ��HIWORD(bitmap.m_lpstr)�������Դ�л��ļ��м���MAKEINTRESOURCE The return value is the specified value in the low-order word and zero in the high-order word. (from msdn)]  
//2 ����DIB  
//	3 ͨ��stbi_load_from_memory���������ļ���ת�����Ƶ�DIB��  
//	4 ����TImageInfo�ṹ  
//	DrawImage()  
//	���� - ����ͼ��  
//	hDC - �豸���  
//	hBitmap - λͼ���  
//	rc - ��������  
//	rcPaint - ��������  
//	rcBmpPart - λͼ����  
//	rcConrners - ��������  
//	alphaChannel - alphaͨ��  
//	uFade - ͸����  
//	hole - ����  
//	xtiled - ����ƽ��  
//	ytiled - ����ƽ��  
//	ʵ�� -   
//	1 CreateCompatibleDC (�����ڴ��豸���)  
//	2 SelectObject() (��λͼѡ���ڴ��豸���)  
//	3 AlphaBlend (alpha���)  
//	4 BitBlt() or StretchBlt() (���Ƶ��ڴ��豸���)  
//	5 SelectObject() (���ϵ�λͼѡ���ڴ��豸���)  
//	6 DeleteDC (�ͷ��ڴ��豸���)  
//	DrawImageString()  
//	���� - ����ʶ�ַ�������ͼ��  
//	hDC - �豸���  
//	pManager - ���ƹ�����  
//	rc - ��������  
//	rcPaint - ��������  
//	pStrImage - ��ʶ�ַ���  
//	pStrModify - ��ʶ�ַ���  
//	ʵ�� -   
//	1 ������ʶ�ַ�����ȡ����  
//	2 ��ȡͼ������  
//	3 ����ͼ��  
//	DrawColor()  
//	���� - ���ƾ���  
//	hDC - �豸���  
//	rc - ��������  
//	color - ��ɫ(alpha, r, g, b)  
//	ʵ�� -  
//	�������Ҫalpha�����SetBkColor() ExtTextOut()  
//	���� ����һ�����ش�С��32λDIB  
//	DrawGradient()  
//	���� - ���ƽ���  
//	hDC - �豸���  
//	rc - ��������  
//	dwFirst - ��ɫ1  
//	dwSecond - ��ɫ2  
//	bVertical - ���䷽�� true��ֱ falseˮƽ  
//	nSteps - ���� [ʹ��GradientFill() APIʱ��Ч]  
//ʵ�� -  
//	1 �������  
//	2 alpha���  
//	DrawLine() - ����ֱ��   
//	DrawRect() - ���ƾ���  
//	DrawRoundRect() - ����Բ�Ǿ���  
//	DrawText() - ��������  
//	DrawHtmlText()  
//	���� - ���� "mini-html" ��ʶ�ַ���  
//	hDC - �豸���  
//	pManager - ���ƹ�����  
//	rc - ��������  
//	pstrText - "mini-html" ��ʶ�ַ���  
//	dwTextColor - �ַ�������  
//	pLinks - ��������Ч����������  
//	sLinks - ��������Ч��������������  
//	nLinkRects - ��������Ч��������������  
//	uStyle - ���ͣ�ͬDrawText() Format Flags  
//	GenerateBitmap()  
//	���� - ����һ��BMPλͼ  
//	pManager - ���ƹ�����  
//	pControl - �ؼ����  
//	rc - ��������  
//	���� - BMP���  