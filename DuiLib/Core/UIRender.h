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
  //加载图片
	static TImageInfo* LoadImage(STRINGorID bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    static void FreeImage(const TImageInfo* bitmap);
    static void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, 
        bool hole = false, bool xtiled = false, bool ytiled = false);
    static bool DrawImageString(HDC hDC, CPaintManagerUI* pManager, const RECT& rcItem, const RECT& rcPaint, 
        LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
	
	//自定义用于动画绘制
	static bool DrawImageAnim(HDC hDC,CPaintManagerUI* pManager,LPCTSTR pStrImage,const RECT& rc,const RECT& rcBmpPart);


    static void DrawColor(HDC hDC, const RECT& rc, DWORD color);
    static void DrawGradient(HDC hDC, const RECT& rc, DWORD dwFirst, DWORD dwSecond, bool bVertical, int nSteps);

    // 以下函数中的颜色参数alpha值无效
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
//	* 渲染引擎  
//	* 函数 -  
//	LoadImage()  
//	功能 - 加载图像  
//	bitmap - 图像  
//	type - 类型  
//	mask - 隐蔽色  
//	实现 -  
//	1 读取文件，资源和zip包中图像文件数据到内存[通过HIWORD(bitmap.m_lpstr)区别从资源中或文件中加载MAKEINTRESOURCE The return value is the specified value in the low-order word and zero in the high-order word. (from msdn)]  
//2 创建DIB  
//	3 通过stbi_load_from_memory加载数据文件并转化后复制到DIB中  
//	4 返回TImageInfo结构  
//	DrawImage()  
//	功能 - 绘制图像  
//	hDC - 设备句柄  
//	hBitmap - 位图句柄  
//	rc - 对象区域  
//	rcPaint - 绘制区域  
//	rcBmpPart - 位图区域  
//	rcConrners - 拉伸区域  
//	alphaChannel - alpha通道  
//	uFade - 透明度  
//	hole - 居中  
//	xtiled - 横向平铺  
//	ytiled - 纵向平铺  
//	实现 -   
//	1 CreateCompatibleDC (创建内存设备句柄)  
//	2 SelectObject() (将位图选入内存设备句柄)  
//	3 AlphaBlend (alpha混合)  
//	4 BitBlt() or StretchBlt() (绘制到内存设备句柄)  
//	5 SelectObject() (将老的位图选入内存设备句柄)  
//	6 DeleteDC (释放内存设备句柄)  
//	DrawImageString()  
//	功能 - 按标识字符串绘制图像  
//	hDC - 设备句柄  
//	pManager - 绘制管理者  
//	rc - 对象区域  
//	rcPaint - 绘制区域  
//	pStrImage - 标识字符串  
//	pStrModify - 标识字符串  
//	实现 -   
//	1 分析标识字符串获取属性  
//	2 获取图像数据  
//	3 绘制图像  
//	DrawColor()  
//	功能 - 绘制矩形  
//	hDC - 设备句柄  
//	rc - 对象区域  
//	color - 颜色(alpha, r, g, b)  
//	实现 -  
//	如果不需要alpha混合则SetBkColor() ExtTextOut()  
//	否则 创建一个像素大小的32位DIB  
//	DrawGradient()  
//	功能 - 绘制渐变  
//	hDC - 设备句柄  
//	rc - 对象区域  
//	dwFirst - 颜色1  
//	dwSecond - 颜色2  
//	bVertical - 渐变方向 true垂直 false水平  
//	nSteps - 步长 [使用GradientFill() API时无效]  
//实现 -  
//	1 渐变填充  
//	2 alpha混合  
//	DrawLine() - 绘制直线   
//	DrawRect() - 绘制矩形  
//	DrawRoundRect() - 绘制圆角矩形  
//	DrawText() - 绘制文字  
//	DrawHtmlText()  
//	功能 - 绘制 "mini-html" 标识字符串  
//	hDC - 设备句柄  
//	pManager - 绘制管理者  
//	rc - 对象区域  
//	pstrText - "mini-html" 标识字符串  
//	dwTextColor - 字符串类型  
//	pLinks - 产生链接效果的区域组  
//	sLinks - 产生链接效果的区域组数量  
//	nLinkRects - 产生链接效果的区域组数量  
//	uStyle - 类型，同DrawText() Format Flags  
//	GenerateBitmap()  
//	功能 - 产生一个BMP位图  
//	pManager - 绘制管理者  
//	pControl - 控件句柄  
//	rc - 对象区域  
//	返回 - BMP句柄  