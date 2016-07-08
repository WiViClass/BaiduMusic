
#pragma once
#include "duilib.h"
#include <deque>
//#include "../_include/xmp/AVPlayer.h"
//#include "libzplay.h"
#include "PlayList.h"
//#include "libzplay.h"
#include "MusicListElement.h"
//#include "..\include\MusicPlayer.h"
#include "..\include\UIAlbum.h"
#include "..\include\trayicon.h"
#include "MusicTest.h"

using namespace libZPlay;



enum PLAY_MSG
{
	MSG_OPEN = 0,    //��
	MSG_PLAY,        //����
	MSG_PLAYING,     //���ڲ���
	MSG_PAUSE,       //��ͣ
	MSG_RESUME,      //����
	MSG_STOP,        //ֹͣ
	MSG_SEEK,        //����λ��
	MSG_PREV,        //��һ��
	MSG_NEXT,        //��һ��
	MSG_VOLUME,      //����
	MSG_SET_LOOP     //����ѭ��״̬
};



enum enum_PlayMode
{
	EM_PLAY_MODE_SEQUENCE,      // ˳�򲥷�
	EM_PLAY_MODE_RANDOM,        // �������
	EM_PLAY_MODE_SINGLE_CIRCLE  // ����ѭ��
};

 
class CDuiFrameWnd: public CXMLWnd
{
public:
	explicit CDuiFrameWnd(LPCTSTR pszXMLName);
	~CDuiFrameWnd();

	virtual void InitWindow();
	bool CreateTrayIcon( WORD nIDResource );
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName);

	void OnDropFiles(HWND hwnd, HDROP hDropInfo);

	bool    OnPosChanged(void* param);                              // ���ȸı䣬�û������ı����
	bool    OnVolumeChanged(void* param);                           // �����ı�

	 TStreamTime GetMusicLenght(CDuiString &MusicPath);
	 MEDIA* GetMusicID3Info(CDuiString &MusicPath);


	//�������Żص�����
	static void player_proc(void *user_data,PLAY_MSG msg,LPARAM lParam);
	static CDuiString TimeToString(int time);

	void PaintAlbum(HDC hDC);
	void SendPlayState();
	void PlayNext();
	void PlayPrev(); 
	void Play(LPCTSTR pszPath);   // ����·��ΪpszPath���ļ�
	void Play();          // ����
	void Play(bool bPlay);	//���Ż���ͣ	
	void Stop();         // ֹͣ
	void OpenFileDialog();                                          // ���ļ�����
	//void ShowPlaylist(bool bShow);                                  // ��ʾ�����б�
	void AddFiles(vector<CDuiString> &fileNames, bool bInit = false);    // ����ļ��������б�
	void AddTest();
	void ElementTest();
	void DelateTest();

	void GetPlayInfo();
	void CloseWnd();
protected:

	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/*
	*/
	virtual void OnClick(TNotifyUI& msg);
	virtual void OnDoubleClick(TNotifyUI& msg);
	virtual void OnItemActivate(TNotifyUI& msg);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam); 

	DUI_DECLARE_MESSAGE_MAP()	

private:
	
	CDuiString      m_strPath;          // ��ǰ�ļ���·��
	bool			IsClose;
	bool			IsReadyPlay;
	bool			m_bPlayState;		//����״̬��־λ��trueΪ�ڲ��� false ��ͣ
	bool			m_bSwitch;			//�ֶ��л���־λ-˫������ʱ��Ϊtrue

	ZPlay			*m_pZplayer;		 //������ָ��,�������Ʋ���
	ZPlay			*m_pSongInfo;		 //������ָ��,��ȡר����Ϣ

	MEDIA			*m_pCurMedia;		 //��ǰ�����ļ�����Ϣ
	CSliderUI       *m_pSliderPlay;     // �ļ����Ž���
	CTextUI         *m_pTextTime;		// �ļ�����ʱ��
	CTextUI         *m_pTextSongName;   // ���ڲ��ŵ��ļ�����
	CButtonUI		*pbtnPlay;			//���Ű�ť-���� 
	CButtonUI		*pbtnPause;			//���Ű�ť-��ͣ
	CButtonUI		*pbtnTrayPlay;		//���Ű�ť-���� ������
	CButtonUI		*pbtnTrayPause;		//���Ű�ť-��ͣ ������

	CMusicListElementUI *m_pCurSong;	//��ǰ���ŵ������ļ�
	CListUI			*m_pCurMusicList;	//��ǰ���ŵ������б�

	CControlUI		*m_pDefaultAlbum;	//Ĭ��ר������
	CAlbumUI		*m_pCurAlbum;		//��ǰר������

	enum_PlayMode   m_emPlayMode;       // ����ģʽ

	std::deque<unsigned>   m_queRand;   // �������ģʽ���ļ����


	void ShowPlayButton(bool bShow);                                // ��ʾ���Ű�ť
	void ShowControlsForPlay(bool bShow);                           // ����ʼ���Ż�ֹͣʱ����ʾ������һЩ�ؼ�


	HWND	mhMenuWnd;
	//CMenuWnd *m_pMainMenu;  ///���ò˵�
};

extern HINSTANCE g_hPcmtInstance;