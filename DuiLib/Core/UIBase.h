/*
UILib�Ļ�����Ķ���Դ�ļ�
����UI���ú�ķ�װ��������Ժ�
��ͼ���õĸ������ݽṹ���Լ�UILib�Ĵ�����
*/


#ifndef __UIBASE_H__
#define __UIBASE_H__


#pragma once

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//
//������ʽ��
#define UI_WNDSTYLE_CONTAINER  (0)     //������ʽ�����κ���ʽ
	//UI��׼��ܴ�����ʽ�����������߿�ͼ�ꡢ���⡢�����С��ť���رհ�ť��
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW) 
	//UI�����ӿؼ���ʽ
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
//UI�Ի�������ʽ
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

//��չ������ʽ��
	//����ͻ���������ʽ
#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
//������������ʽ+ģʽ�Ի�����ʽ
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

	//����ʽ��

#define UI_CLASSSTYLE_CONTAINER  (0)
	//ˮƽ��ֱ��С�ı�ʱ����
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)


/////////////////////////////////////////////////////////////////////////////////////
//
#ifndef ASSERT   
	 //�жϱ��ʽ�Ƿ�ΪNULL��FALSE
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
���ڶ��������
��������
������Ϣ���̴���
�ṩ�������໯�볬�໯�ӿ�

*/


class UILIB_API CWindowWnd
{
public:
    CWindowWnd();

    HWND GetHWND() const;
    operator HWND() const;
//ע�ᴰ�ڹ���
	//����GetClassStyle��ȡ��ʽ��Ϣ
	//���û�ͼ�������ľ�̬������ȡʵ��CPainterManagerUI::GetInstance
	//����GetWindowClassName������ȡҪע���������
    bool RegisterWindowClass();
	//ͨ���Ѿ����ڵĴ�����Ϊ����ע�ᴰ����
    bool RegisterSuperclass();

    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
    //��������
	//������ڸ�����������ע�Ḹ��������
	//�����ڸ����������������Ĵ��ڴ���
	//����ע��ɹ��󴴽�����
	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
	//
	HWND CreateDuiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle =0, DWORD dwExStyle =0);
    HWND Subclass(HWND hWnd);//�������໯
    void Unsubclass();//ж�ش������໯
	//��ʾ����
    void ShowWindow(bool bShow = true, bool bTakeFocus = true);
    UINT ShowModal();//��ʾģʽ����
    void Close(UINT nRet = IDOK);//�رմ���
    void CenterWindow();	// ���У�֧����չ��Ļ���Ӵ��ھ��ڸ����ڵ�����
    void SetIcon(UINT nRes);
	//����UI��Ϣ
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    //Ͷ��UI�첽��Ϣ
	LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    void ResizeClient(int cx = -1, int cy = -1);

protected:
    virtual LPCTSTR GetWindowClassName() const = 0;
    virtual LPCTSTR GetSuperClassName() const;
    virtual UINT GetClassStyle() const;
	//��Ϣ������
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��������ǰ���Ĵ�����
	virtual void OnFinalMessage(HWND hWnd);
	//���ڹ��̻ص�����
    static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ؼ����̻ص�����
	static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    HWND m_hWnd;  //��������ʼ��ΪNULL
    WNDPROC m_OldWndProc;//Win32���ڹ��̣�Ĭ��ΪDefWindowProc
    bool m_bSubclassed; //�Ƿ����໯��Ĭ��False
};

} // namespace DuiLib

#endif // __UIBASE_H__
