#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//

class CControlUI;


/////////////////////////////////////////////////////////////////////////////////////
//

typedef enum EVENTTYPE_UI
{
    UIEVENT__FIRST = 1,
    UIEVENT__KEYBEGIN,
    UIEVENT_KEYDOWN,
    UIEVENT_KEYUP,
    UIEVENT_CHAR,
    UIEVENT_SYSKEY,
    UIEVENT__KEYEND,
    UIEVENT__MOUSEBEGIN,
    UIEVENT_MOUSEMOVE,
	UIEVENT_ITEMMOUSEMOVE,
    UIEVENT_MOUSELEAVE,
    UIEVENT_MOUSEENTER,
    UIEVENT_MOUSEHOVER,
    UIEVENT_BUTTONDOWN,
    UIEVENT_BUTTONUP,
    UIEVENT_RBUTTONDOWN,
    UIEVENT_DBLCLICK,
    UIEVENT_CONTEXTMENU,
    UIEVENT_SCROLLWHEEL,
    UIEVENT__MOUSEEND,
    UIEVENT_KILLFOCUS,
    UIEVENT_SETFOCUS,
    UIEVENT_WINDOWSIZE,
    UIEVENT_SETCURSOR,
    UIEVENT_TIMER,
    UIEVENT_NOTIFY,
    UIEVENT_COMMAND,
    UIEVENT__LAST,
};

/////////////////////////////////////////////////////////////////////////////////////
//

// Flags for CControlUI::GetControlFlags()
#define UIFLAG_TABSTOP       0x00000001
#define UIFLAG_SETCURSOR     0x00000002
#define UIFLAG_WANTRETURN    0x00000004

// Flags for FindControl()
#define UIFIND_ALL           0x00000000
#define UIFIND_VISIBLE       0x00000001
#define UIFIND_ENABLED       0x00000002
#define UIFIND_HITTEST       0x00000004
#define UIFIND_TOP_FIRST     0x00000008
#define UIFIND_ME_FIRST      0x80000000

// Flags for the CDialogLayout stretching
#define UISTRETCH_NEWGROUP   0x00000001
#define UISTRETCH_NEWLINE    0x00000002
#define UISTRETCH_MOVE_X     0x00000004
#define UISTRETCH_MOVE_Y     0x00000008
#define UISTRETCH_SIZE_X     0x00000010
#define UISTRETCH_SIZE_Y     0x00000020

// Flags used for controlling the paint
#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040
#define UISTATE_ACTIVE		 0x00000080



// Flags used for controlling the Animation style
#define UIANIM_REPEAT_ONE      0x00000001
#define UIANIM_REPEAT_ARRAY     0x00000002
//#define UIANIM_DISABLED     0x00000004
//#define UIANIM_HOT          0x00000008
//#define UIANIM_PUSHED       0x00000010
//#define UIANIM_READONLY     0x00000020
//#define UIANIM_CAPTURED     0x00000040
//#define UIANIM_ACTIVE		 0x00000080
/////////////////////////////////////////////////////////////////////////////////////
//

typedef struct tagTFontInfo
{
    HFONT hFont;
    CDuiString sFontName;
    int iSize;
    bool bBold;
    bool bUnderline;
    bool bItalic;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
    bool alphaChannel;
    CDuiString sResType;
    DWORD dwMask;
} TImageInfo;

// Structure for notifications from the system
// to the control implementation.
typedef struct tagTEventUI
{
    int Type;
    CControlUI* pSender;
    DWORD dwTimestamp;
    POINT ptMouse;
    TCHAR chKey;
    WORD wKeyState;
    WPARAM wParam;
    LPARAM lParam;
} TEventUI;

// Structure for relative position to the parent
typedef struct tagTRelativePosUI
{
	bool bRelative;
	SIZE szParent;
	int nMoveXPercent;
	int nMoveYPercent;
	int nZoomXPercent;
	int nZoomYPercent;
}TRelativePosUI;

// Listener interface
//事件通知抽象类
//重在Notify虚函数，处理事件通知
class INotifyUI
{
public:
    virtual void Notify(TNotifyUI& msg) = 0;
};

// MessageFilter interface
class IMessageFilterUI
{
public:
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) = 0;
};

class ITranslateAccelerator
{
public:
	virtual LRESULT TranslateAccelerator(MSG *pMsg) = 0;
};


/////////////////////////////////////////////////////////////////////////////////////
//
typedef CControlUI* (*LPCREATECONTROL)(LPCTSTR pstrType);
/*
窗口消息及图形绘制管理器类，与窗口绑定
绘制控件
消息管理
事件通知
*/

class UILIB_API CPaintManagerUI
{
public:
    CPaintManagerUI();
    ~CPaintManagerUI();

public:
	//绘图管理器初始化，（m_hWndPaint，m_hDcPaint赋值，在预处理消息中加入管理器）
    void Init(HWND hWnd);
	//当前需要更新界面
    void NeedUpdate();
	//指定区域失效
    void Invalidate(RECT& rcItem);
	//获取绘图设备DC
    HDC GetPaintDC() const;
	//获取绘图的窗口句柄
    HWND GetPaintWindow() const;
	//获取提示窗体的句柄
    HWND GetTooltipWindow() const;

    POINT GetMousePos() const;
	//获取客户区大小
    SIZE GetClientSize() const;
	//获取窗体初始化大小
    SIZE GetInitSize();
	//设置大小
    void SetInitSize(int cx, int cy);
    RECT& GetSizeBox();
    void SetSizeBox(RECT& rcSizeBox);
    RECT& GetCaptionRect();
    void SetCaptionRect(RECT& rcCaption);
    SIZE GetRoundCorner() const;
    void SetRoundCorner(int cx, int cy);
    SIZE GetMinInfo() const;
    void SetMinInfo(int cx, int cy);
    SIZE GetMaxInfo() const;
    void SetMaxInfo(int cx, int cy);
	//窗体不透明度（0完全透明，255完全不透明）
	int GetTransparent() const;
    void SetTransparent(int nOpacity);
    void SetBackgroundTransparent(bool bTrans);
	//
    bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);
	//获取当前管理的实例句柄
    static HINSTANCE GetInstance();
	//获取当前运行的实例的路径
    static CDuiString GetInstancePath();
	//获取当前的工作路径
    static CDuiString GetCurrentPath();
	//获取资源的DLL实例句柄
    static HINSTANCE GetResourceDll();
	//
    static const CDuiString& GetResourcePath();
    static const CDuiString& GetResourceZip();
    static bool IsCachedResourceZip();
    static HANDLE GetResourceZipHandle();
    static void SetInstance(HINSTANCE hInst);
    static void SetCurrentPath(LPCTSTR pStrPath);
    static void SetResourceDll(HINSTANCE hInst);
    static void SetResourcePath(LPCTSTR pStrPath);
	static void SetResourceZip(LPVOID pVoid, unsigned int len);
    static void SetResourceZip(LPCTSTR pstrZip, bool bCachedResourceZip = false);
    static void GetHSL(short* H, short* S, short* L);
    static void SetHSL(bool bUseHSL, short H, short S, short L); // H:0~360, S:0~200, L:0~200 
    static void ReloadSkin();
    static bool LoadPlugin(LPCTSTR pstrModuleName);
    static CStdPtrArray* GetPlugins();
	//设置使用上级资源的绘图管理器
    bool UseParentResource(CPaintManagerUI* pm);
    CPaintManagerUI* GetParentResource() const;


	//字体颜色
    DWORD GetDefaultDisabledColor() const;
    void SetDefaultDisabledColor(DWORD dwColor);
    DWORD GetDefaultFontColor() const;
    void SetDefaultFontColor(DWORD dwColor);
    DWORD GetDefaultLinkFontColor() const;
    void SetDefaultLinkFontColor(DWORD dwColor);
    DWORD GetDefaultLinkHoverFontColor() const;
    void SetDefaultLinkHoverFontColor(DWORD dwColor);
    DWORD GetDefaultSelectedBkColor() const;
    void SetDefaultSelectedBkColor(DWORD dwColor);
    TFontInfo* GetDefaultFontInfo();
    void SetDefaultFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    DWORD GetCustomFontCount() const;
    HFONT AddFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT AddFontAt(int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    HFONT GetFont(int index);
    HFONT GetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool FindFont(HFONT hFont);
    bool FindFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    int GetFontIndex(HFONT hFont);
    int GetFontIndex(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic);
    bool RemoveFont(HFONT hFont);
    bool RemoveFontAt(int index);
    void RemoveAllFonts();
    TFontInfo* GetFontInfo(int index);
    TFontInfo* GetFontInfo(HFONT hFont);
	//根据图片路径查找图像信息
    const TImageInfo* GetImage(LPCTSTR bitmap);
	//根据名称，类型，遮罩色，查询图像信息
    const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    bool RemoveImage(LPCTSTR bitmap);
    void RemoveAllImages();
    void ReloadAllImages();
	//添加控件的默认配置信息
    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    //根据控件名称查询该类控件的默认配置
	LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
	//移除指定控件的默认配置
    bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
	//获取默认配置信息列表
    const CStdStringPtrMap& GetDefaultAttribultes() const;
	//清空默认配置信息列表
    void RemoveAllDefaultAttributeList();
	//将对话框控件附加到当前的管理器中
    bool AttachDialog(CControlUI* pControl);
	//控件初始化
    bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    //控件回收
	void ReapObjects(CControlUI* pControl);
	//添加控件到指定的选项组
    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    //查询指定选项组的全部选项
	CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();
	//获取焦点状态的控件
    CControlUI* GetFocus() const;
	//设置控件为获取焦点状态
	void SetFocus(CControlUI* pControl);
	//设置焦点为需要绘制焦点 
	void SetFocusNeeded(CControlUI* pControl);
	//设置下一个获得Tab键会获得焦点的控件，Tab是否继续往下走
    bool SetNextTabControl(bool bForward = true);
	//为指定控件以及其子控件设置定时器
    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    // 移除指定控件上的指定编号的定时器
	bool KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();
	//设置窗体接受鼠标事件
    void SetCapture();
	//释放窗体捕获鼠标事件
    void ReleaseCapture();
	//判断窗体是否接受鼠标事件
    bool IsCaptured();
	//添加控件到通知集合中
    bool AddNotifier(INotifyUI* pControl);
	//将控件从通知集合中移除
    bool RemoveNotifier(INotifyUI* pControl);  
	//发送同步/异步通知
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
	//构建同步或异步通知并发送
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);
	// 向预处理消息过滤器链中添加消息过滤器
    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    bool RemovePreMessageFilter(IMessageFilterUI* pFilter);
	//向消息过滤器链中添加消息过滤器
    bool AddMessageFilter(IMessageFilterUI* pFilter);
    bool RemoveMessageFilter(IMessageFilterUI* pFilter);

    int GetPostPaintCount() const;
    bool AddPostPaint(CControlUI* pControl);
    bool RemovePostPaint(CControlUI* pControl);
    bool SetPostPaintIndex(CControlUI* pControl, int iIndex);

    void AddDelayedCleanup(CControlUI* pControl);

	bool AddTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool RemoveTranslateAccelerator(ITranslateAccelerator *pTranslateAccelerator);
	bool TranslateAccelerator(LPMSG pMsg);

    CControlUI* GetRoot() const;
    CControlUI* FindControl(POINT pt) const;
    CControlUI* FindControl(LPCTSTR pstrName) const;
    CControlUI* FindSubControlByPoint(CControlUI* pParent, POINT pt) const;
    CControlUI* FindSubControlByName(CControlUI* pParent, LPCTSTR pstrName) const;
    CControlUI* FindSubControlByClass(CControlUI* pParent, LPCTSTR pstrClass, int iIndex = 0);
    CStdPtrArray* FindSubControlsByClass(CControlUI* pParent, LPCTSTR pstrClass);
    CStdPtrArray* GetSubControlsByClass();
	// 消息循环，非游戏框架消息泵，无法利用无消息的空闲时间
    static void MessageLoop();
	//消息翻译，在Win32原有的消息转换基础上，将需要自己处理的消息转发给消息预处理器
    static bool TranslateMessage(const LPMSG pMsg);
	static void Term();
//消息处理器
//处理系统消息、将系统消息转化为事件交给控件处理
//部分消息如滑轮滚动调用SendMessage
//SendMessage将指定的消息发送到一个或多个窗口。
//直到窗口程序处理完消息再返回。
//PostMessage是将一个消息寄送到一个线程的消息队列后就立即返回。
//控件收到后再分发到具体控件处理
//具体控件收到进行部分处理并发送异步消息

    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	//消息预处理器
		//1.消息预处理过滤(消息预处理过滤器集合对消息进行过滤处理)
		//2.检查是否按下Tab键，设置下一个获得焦点的控件
		//3.处理Alt+Shortcut Key按下后的控件获得焦点和激活的设置
		//4.检查是否有系统键消息，有则发送获得焦点的控件的事件
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	void UsedVirtualWnd(bool bUsed);

private:
	//将所有控件添加到m_mNameHash哈希表中
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    //计算控件数量
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    //根据点是否在区域中查询控件
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    //通过TAB信息查找控件
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    //通过快照查询控件
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    //查找需要更新的控件
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
    //通过名称查找控件
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:
	//窗体句柄
    HWND m_hWndPaint;//要CPaintManagerUI进行Direct绘图操作的窗口句柄
	int m_nOpacity;
    HDC m_hDcPaint;//直接绘图到窗体的DC（窗口整个区域包括非客户区域）
    HDC m_hDcOffscreen;//内存缓冲区绘图DC
    HDC m_hDcBackground;//背景绘制（支持AlphaBackground时使用）
    HBITMAP m_hbmpOffscreen;//m_hDcPaint的后台作画画布
    HBITMAP m_hbmpBackground;//背景图片BMP
    HWND m_hwndTooltip;//提示窗口句柄
    TOOLINFO m_ToolTip;
    bool m_bShowUpdateRect;//是否显示更新区域
    //控件信息
    CControlUI* m_pRoot;  //XML根节点解析成的对象，通常为各种Window
    CControlUI* m_pFocus;//处于获得焦点状态的控件
    CControlUI* m_pEventHover;//处于鼠标悬停状态下的控件
    CControlUI* m_pEventClick;//被鼠标点击的控件
    CControlUI* m_pEventKey;//接受键盘输入的控件
    //位置记录信息
    POINT m_ptLastMousePos;//鼠标最新位置
    SIZE m_szMinWindow;//
    SIZE m_szMaxWindow;
    SIZE m_szInitWindowSize;
    RECT m_rcSizeBox;//窗体外边框区域的大小
    SIZE m_szRoundCorner;//四角的圆角弧度
    RECT m_rcCaption;//标题栏区域
    UINT m_uTimerID;//当前定时器
    bool m_bFirstLayout;//是否是首个布局
    bool m_bUpdateNeeded;//是否需要更新界面
    bool m_bFocusNeeded;//是否需要焦点
    bool m_bOffscreenPaint;//是否需要开双缓存绘图
    bool m_bAlphaBackground;//窗体背景是否需要支持Alpha通道（如png的半透明效果）
    bool m_bMouseTracking;//是否需要支持鼠标追踪
    bool m_bMouseCapture;//是否支持鼠标捕获
	bool m_bUsedVirtualWnd;

    //集合类信息
    CStdPtrArray m_aNotifiers;//能够接受通知的对象集合
    CStdPtrArray m_aTimers;//定时器集合
    CStdPtrArray m_aPreMessageFilters;//预处理消息集合
    CStdPtrArray m_aMessageFilters;//消息过滤器集合
    CStdPtrArray m_aPostPaintControls;//发送绘制请求的控件集合
    CStdPtrArray m_aDelayedCleanup;//延迟清理的对象集合
    CStdPtrArray m_aAsyncNotify;//异步通知的消息集合
    CStdPtrArray m_aFoundControls;//
    CStdStringPtrMap m_mNameHash;//名称HASHMAP
    CStdStringPtrMap m_mOptionGroup;//选项组map
    //XML对应的资源
    CPaintManagerUI* m_pParentResourcePM;//父类资源的PaintManagerUI绘图管理器
    DWORD m_dwDefaultDisabledColor;//
    DWORD m_dwDefaultFontColor;
    DWORD m_dwDefaultLinkFontColor;
    DWORD m_dwDefaultLinkHoverFontColor;
    DWORD m_dwDefaultSelectedBkColor;
    TFontInfo m_DefaultFontInfo;
    CStdPtrArray m_aCustomFonts;

    CStdStringPtrMap m_mImageHash;
    CStdStringPtrMap m_DefaultAttrHash;
    //
    static HINSTANCE m_hInstance;//当前管理的Instance实例
    static HINSTANCE m_hResourceInstance;//当前管理的资源DLLInstance实例
    static CDuiString m_pStrResourcePath;//当前使用的资源路径
    static CDuiString m_pStrResourceZip;//当前使用的资源压缩包文件全称
    static bool m_bCachedResourceZip;
    static HANDLE m_hResourceZip;
    static short m_H;
    static short m_S;
    static short m_L;
    static CStdPtrArray m_aPreMessages;
    static CStdPtrArray m_aPlugins;

public:
	static CDuiString m_pStrDefaultFontName;
	CStdPtrArray m_aTranslateAccelerator;
};

} // namespace DuiLib

#endif // __UIMANAGER_H__
