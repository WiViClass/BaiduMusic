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
//�¼�֪ͨ������
//����Notify�麯���������¼�֪ͨ
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
������Ϣ��ͼ�λ��ƹ������࣬�봰�ڰ�
���ƿؼ�
��Ϣ����
�¼�֪ͨ
*/

class UILIB_API CPaintManagerUI
{
public:
    CPaintManagerUI();
    ~CPaintManagerUI();

public:
	//��ͼ��������ʼ������m_hWndPaint��m_hDcPaint��ֵ����Ԥ������Ϣ�м����������
    void Init(HWND hWnd);
	//��ǰ��Ҫ���½���
    void NeedUpdate();
	//ָ������ʧЧ
    void Invalidate(RECT& rcItem);
	//��ȡ��ͼ�豸DC
    HDC GetPaintDC() const;
	//��ȡ��ͼ�Ĵ��ھ��
    HWND GetPaintWindow() const;
	//��ȡ��ʾ����ľ��
    HWND GetTooltipWindow() const;

    POINT GetMousePos() const;
	//��ȡ�ͻ�����С
    SIZE GetClientSize() const;
	//��ȡ�����ʼ����С
    SIZE GetInitSize();
	//���ô�С
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
	//���岻͸���ȣ�0��ȫ͸����255��ȫ��͸����
	int GetTransparent() const;
    void SetTransparent(int nOpacity);
    void SetBackgroundTransparent(bool bTrans);
	//
    bool IsShowUpdateRect() const;
    void SetShowUpdateRect(bool show);
	//��ȡ��ǰ�����ʵ�����
    static HINSTANCE GetInstance();
	//��ȡ��ǰ���е�ʵ����·��
    static CDuiString GetInstancePath();
	//��ȡ��ǰ�Ĺ���·��
    static CDuiString GetCurrentPath();
	//��ȡ��Դ��DLLʵ�����
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
	//����ʹ���ϼ���Դ�Ļ�ͼ������
    bool UseParentResource(CPaintManagerUI* pm);
    CPaintManagerUI* GetParentResource() const;


	//������ɫ
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
	//����ͼƬ·������ͼ����Ϣ
    const TImageInfo* GetImage(LPCTSTR bitmap);
	//�������ƣ����ͣ�����ɫ����ѯͼ����Ϣ
    const TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    
	const TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    const TImageInfo* AddImage(LPCTSTR bitmap, HBITMAP hBitmap, int iWidth, int iHeight, bool bAlpha);
    bool RemoveImage(LPCTSTR bitmap);
    void RemoveAllImages();
    void ReloadAllImages();
	//��ӿؼ���Ĭ��������Ϣ
    void AddDefaultAttributeList(LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList);
    //���ݿؼ����Ʋ�ѯ����ؼ���Ĭ������
	LPCTSTR GetDefaultAttributeList(LPCTSTR pStrControlName) const;
	//�Ƴ�ָ���ؼ���Ĭ������
    bool RemoveDefaultAttributeList(LPCTSTR pStrControlName);
	//��ȡĬ��������Ϣ�б�
    const CStdStringPtrMap& GetDefaultAttribultes() const;
	//���Ĭ��������Ϣ�б�
    void RemoveAllDefaultAttributeList();
	//���Ի���ؼ����ӵ���ǰ�Ĺ�������
    bool AttachDialog(CControlUI* pControl);
	//�ؼ���ʼ��
    bool InitControls(CControlUI* pControl, CControlUI* pParent = NULL);
    //�ؼ�����
	void ReapObjects(CControlUI* pControl);
	//��ӿؼ���ָ����ѡ����
    bool AddOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    //��ѯָ��ѡ�����ȫ��ѡ��
	CStdPtrArray* GetOptionGroup(LPCTSTR pStrGroupName);
    void RemoveOptionGroup(LPCTSTR pStrGroupName, CControlUI* pControl);
    void RemoveAllOptionGroups();
	//��ȡ����״̬�Ŀؼ�
    CControlUI* GetFocus() const;
	//���ÿؼ�Ϊ��ȡ����״̬
	void SetFocus(CControlUI* pControl);
	//���ý���Ϊ��Ҫ���ƽ��� 
	void SetFocusNeeded(CControlUI* pControl);
	//������һ�����Tab�����ý���Ŀؼ���Tab�Ƿ����������
    bool SetNextTabControl(bool bForward = true);
	//Ϊָ���ؼ��Լ����ӿؼ����ö�ʱ��
    bool SetTimer(CControlUI* pControl, UINT nTimerID, UINT uElapse);
    // �Ƴ�ָ���ؼ��ϵ�ָ����ŵĶ�ʱ��
	bool KillTimer(CControlUI* pControl, UINT nTimerID);
    void KillTimer(CControlUI* pControl);
    void RemoveAllTimers();
	//���ô����������¼�
    void SetCapture();
	//�ͷŴ��岶������¼�
    void ReleaseCapture();
	//�жϴ����Ƿ��������¼�
    bool IsCaptured();
	//��ӿؼ���֪ͨ������
    bool AddNotifier(INotifyUI* pControl);
	//���ؼ���֪ͨ�������Ƴ�
    bool RemoveNotifier(INotifyUI* pControl);  
	//����ͬ��/�첽֪ͨ
    void SendNotify(TNotifyUI& Msg, bool bAsync = false);
	//����ͬ�����첽֪ͨ������
    void SendNotify(CControlUI* pControl, LPCTSTR pstrMessage, WPARAM wParam = 0, LPARAM lParam = 0, bool bAsync = false);
	// ��Ԥ������Ϣ���������������Ϣ������
    bool AddPreMessageFilter(IMessageFilterUI* pFilter);
    bool RemovePreMessageFilter(IMessageFilterUI* pFilter);
	//����Ϣ���������������Ϣ������
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
	// ��Ϣѭ��������Ϸ�����Ϣ�ã��޷���������Ϣ�Ŀ���ʱ��
    static void MessageLoop();
	//��Ϣ���룬��Win32ԭ�е���Ϣת�������ϣ�����Ҫ�Լ��������Ϣת������ϢԤ������
    static bool TranslateMessage(const LPMSG pMsg);
	static void Term();
//��Ϣ������
//����ϵͳ��Ϣ����ϵͳ��Ϣת��Ϊ�¼������ؼ�����
//������Ϣ�绬�ֹ�������SendMessage
//SendMessage��ָ������Ϣ���͵�һ���������ڡ�
//ֱ�����ڳ���������Ϣ�ٷ��ء�
//PostMessage�ǽ�һ����Ϣ���͵�һ���̵߳���Ϣ���к���������ء�
//�ؼ��յ����ٷַ�������ؼ�����
//����ؼ��յ����в��ִ��������첽��Ϣ

    bool MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	//��ϢԤ������
		//1.��ϢԤ�������(��ϢԤ������������϶���Ϣ���й��˴���)
		//2.����Ƿ���Tab����������һ����ý���Ŀؼ�
		//3.����Alt+Shortcut Key���º�Ŀؼ���ý���ͼ��������
		//4.����Ƿ���ϵͳ����Ϣ�������ͻ�ý���Ŀؼ����¼�
    bool PreMessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lRes);
	void UsedVirtualWnd(bool bUsed);

private:
	//�����пؼ���ӵ�m_mNameHash��ϣ����
    static CControlUI* CALLBACK __FindControlFromNameHash(CControlUI* pThis, LPVOID pData);
    //����ؼ�����
	static CControlUI* CALLBACK __FindControlFromCount(CControlUI* pThis, LPVOID pData);
    //���ݵ��Ƿ��������в�ѯ�ؼ�
	static CControlUI* CALLBACK __FindControlFromPoint(CControlUI* pThis, LPVOID pData);
    //ͨ��TAB��Ϣ���ҿؼ�
	static CControlUI* CALLBACK __FindControlFromTab(CControlUI* pThis, LPVOID pData);
    //ͨ�����ղ�ѯ�ؼ�
	static CControlUI* CALLBACK __FindControlFromShortcut(CControlUI* pThis, LPVOID pData);
    //������Ҫ���µĿؼ�
	static CControlUI* CALLBACK __FindControlFromUpdate(CControlUI* pThis, LPVOID pData);
    //ͨ�����Ʋ��ҿؼ�
	static CControlUI* CALLBACK __FindControlFromName(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlFromClass(CControlUI* pThis, LPVOID pData);
    static CControlUI* CALLBACK __FindControlsFromClass(CControlUI* pThis, LPVOID pData);

private:
	//������
    HWND m_hWndPaint;//ҪCPaintManagerUI����Direct��ͼ�����Ĵ��ھ��
	int m_nOpacity;
    HDC m_hDcPaint;//ֱ�ӻ�ͼ�������DC������������������ǿͻ�����
    HDC m_hDcOffscreen;//�ڴ滺������ͼDC
    HDC m_hDcBackground;//�������ƣ�֧��AlphaBackgroundʱʹ�ã�
    HBITMAP m_hbmpOffscreen;//m_hDcPaint�ĺ�̨��������
    HBITMAP m_hbmpBackground;//����ͼƬBMP
    HWND m_hwndTooltip;//��ʾ���ھ��
    TOOLINFO m_ToolTip;
    bool m_bShowUpdateRect;//�Ƿ���ʾ��������
    //�ؼ���Ϣ
    CControlUI* m_pRoot;  //XML���ڵ�����ɵĶ���ͨ��Ϊ����Window
    CControlUI* m_pFocus;//���ڻ�ý���״̬�Ŀؼ�
    CControlUI* m_pEventHover;//���������ͣ״̬�µĿؼ�
    CControlUI* m_pEventClick;//��������Ŀؼ�
    CControlUI* m_pEventKey;//���ܼ�������Ŀؼ�
    //λ�ü�¼��Ϣ
    POINT m_ptLastMousePos;//�������λ��
    SIZE m_szMinWindow;//
    SIZE m_szMaxWindow;
    SIZE m_szInitWindowSize;
    RECT m_rcSizeBox;//������߿�����Ĵ�С
    SIZE m_szRoundCorner;//�Ľǵ�Բ�ǻ���
    RECT m_rcCaption;//����������
    UINT m_uTimerID;//��ǰ��ʱ��
    bool m_bFirstLayout;//�Ƿ����׸�����
    bool m_bUpdateNeeded;//�Ƿ���Ҫ���½���
    bool m_bFocusNeeded;//�Ƿ���Ҫ����
    bool m_bOffscreenPaint;//�Ƿ���Ҫ��˫�����ͼ
    bool m_bAlphaBackground;//���屳���Ƿ���Ҫ֧��Alphaͨ������png�İ�͸��Ч����
    bool m_bMouseTracking;//�Ƿ���Ҫ֧�����׷��
    bool m_bMouseCapture;//�Ƿ�֧����겶��
	bool m_bUsedVirtualWnd;

    //��������Ϣ
    CStdPtrArray m_aNotifiers;//�ܹ�����֪ͨ�Ķ��󼯺�
    CStdPtrArray m_aTimers;//��ʱ������
    CStdPtrArray m_aPreMessageFilters;//Ԥ������Ϣ����
    CStdPtrArray m_aMessageFilters;//��Ϣ����������
    CStdPtrArray m_aPostPaintControls;//���ͻ�������Ŀؼ�����
    CStdPtrArray m_aDelayedCleanup;//�ӳ�����Ķ��󼯺�
    CStdPtrArray m_aAsyncNotify;//�첽֪ͨ����Ϣ����
    CStdPtrArray m_aFoundControls;//
    CStdStringPtrMap m_mNameHash;//����HASHMAP
    CStdStringPtrMap m_mOptionGroup;//ѡ����map
    //XML��Ӧ����Դ
    CPaintManagerUI* m_pParentResourcePM;//������Դ��PaintManagerUI��ͼ������
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
    static HINSTANCE m_hInstance;//��ǰ�����Instanceʵ��
    static HINSTANCE m_hResourceInstance;//��ǰ�������ԴDLLInstanceʵ��
    static CDuiString m_pStrResourcePath;//��ǰʹ�õ���Դ·��
    static CDuiString m_pStrResourceZip;//��ǰʹ�õ���Դѹ�����ļ�ȫ��
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
