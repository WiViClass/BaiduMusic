/*
UILib的基础类的定义源文件
包括UI常用宏的封装、代码调试宏
绘图常用的各种数据结构，以及UILib的窗口类
*/


#ifndef __UIBASE_H__
#define __UIBASE_H__


#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//
//窗体样式宏
#define UI_WNDSTYLE_CONTAINER  (0)     //容器样式、无任何样式
	//UI标准框架窗体样式（标题栏、边框、图标、标题、最大、最小按钮、关闭按钮）
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW) 
	//UI窗体子控件样式
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
//UI对话框窗体样式
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

//扩展窗体样式宏
	//窗体客户区凹陷样式
#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
//工具栏窗口样式+模式对话框样式
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

	//类样式宏

#define UI_CLASSSTYLE_CONTAINER  (0)
	//水平垂直大小改变时绘制
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)


/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT   
	 //判断表达式是否为NULL或FALSE
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef DUITRACE
#define DUITRACE DUI__Trace
#endif
#define DUITRACEMSG DUI__TraceMsg
#else
#ifndef DUITRACE
#define DUITRACE
#endif
#define DUITRACEMSG _T("")
#endif

void UILIB_API DUI__Trace(LPCTSTR pstrFormat, ...);
LPCTSTR UILIB_API DUI__TraceMsg(UINT uMsg);

/////////////////////////////////////////////////////////////////////////////////////
//

class UILIB_API CNotifyPump
{
public:
	bool AddVirtualWnd(CDuiString strName,CNotifyPump* pObject);
	bool RemoveVirtualWnd(CDuiString strName);
	void NotifyPump(TNotifyUI& msg);
	bool LoopDispatch(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP()
private:
	CStdStringPtrMap m_VirtualWndMap;
};

/*
窗口对象管理父类
创建窗口
窗口消息过程处理
提供窗口子类化与超类化接口

*/


class UILIB_API CWindowWnd
{
public:
    CWindowWnd();

    HWND GetHWND() const;
    operator HWND() const;
//注册窗口过程
	//调用GetClassStyle获取样式信息
	//调用绘图管理器的静态函数获取实例CPainterManagerUI::GetInstance
	//调用GetWindowClassName（）获取要注册的类名称
    bool RegisterWindowClass();
	//通过已经存在的窗体作为父类注册窗口类
    bool RegisterSuperclass();

    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    //创建窗口
	//如果存在父类名，就用注册父类来创建
	//不存在父类名，进行正常的窗口创建
	//以上注册成功后创建窗口
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	//
	HWND CreateDuiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle =0, DWORD dwExStyle =0);
    HWND Subclass(HWND hWnd);//窗口子类化
    void Unsubclass();//卸载窗口子类化
	//显示窗口
    void ShowWindow(bool bShow = true, bool bTakeFocus = true);
    UINT ShowModal();//显示模式窗口
    void Close(UINT nRet = IDOK);//关闭窗口
    void CenterWindow();	// 居中，支持扩展屏幕，子窗口局于父窗口的中央
    void SetIcon(UINT nRes);
	//发送UI消息
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    //投递UI异步消息
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    void ResizeClient(int cx = -1, int cy = -1);

protected:
    virtual LPCTSTR GetWindowClassName() const = 0;
    virtual LPCTSTR GetSuperClassName() const;
    virtual UINT GetClassStyle() const;
	//消息处理函数
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗体销毁前最后的处理工作
	virtual void OnFinalMessage(HWND hWnd);
	//窗口过程回调函数
    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //控件过程回调函数
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;  //窗体句柄初始化为NULL
    WNDPROC m_OldWndProc;//Win32窗口过程，默认为DefWindowProc
    bool m_bSubclassed; //是否子类化，默认False
};

} // namespace DuiLib

#endif // __UIBASE_H__
