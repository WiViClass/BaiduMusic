

#pragma once



namespace DuiLib
{
	class UILIB_API CAnimate : public CControlUI
{
	public:
		CAnimate();
		~CAnimate();
	LPCTSTR	GetClass();
	LPVOID	GetInterface(LPCTSTR pstrName);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void PaintText(HDC hDC);

	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DrawAnimate(HDC hDC, const RECT& rcItem);

	void SetStartImage(LPCTSTR pStrImage);
	void SetEndImage(LPCTSTR pStrImage);
	void SetPauseImage(LPCTSTR pStrImage);
	void SetAnimImage(LPCTSTR pStrImage);

	void SetSource(RECT rc);

	void SetAnimStart(int iStart);
	void SetAnimStep(int iStep);
	void SetAnimEnd(int iEnd);

	LPCTSTR GetStartImage()const;
	LPCTSTR GetEndImage()const;
	LPCTSTR GetPauseImage()const;
	LPCTSTR GetAnimImage()const;

	RECT GetSource()const;

	int GetAnimStart()const;
	int GetAnimStep()const;
	int GetAnimEnd()const;

	void AnimSetting(int istart,int istep,int iend);
	void PlayAnim();
	void SetAnimSource(RECT rc);
		
	public:

	 UINT m_uAnimStyle;
	 int AnimCount;
	 CDuiString m_sStartImage;
	 CDuiString m_sEndImage;
	 CDuiString m_sPauseImage;
	 CDuiString m_sAnimImage;

    RECT m_rcAnimSource;
    RECT m_rcItemSource;

	int m_animStart;
	int m_animStep;
	int m_animEnd;

	};
}

