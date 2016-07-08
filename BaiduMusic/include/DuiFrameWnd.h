
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
	MSG_OPEN = 0,    //打开
	MSG_PLAY,        //播放
	MSG_PLAYING,     //正在播放
	MSG_PAUSE,       //暂停
	MSG_RESUME,      //继续
	MSG_STOP,        //停止
	MSG_SEEK,        //播放位置
	MSG_PREV,        //上一首
	MSG_NEXT,        //下一首
	MSG_VOLUME,      //音量
	MSG_SET_LOOP     //设置循环状态
};



enum enum_PlayMode
{
	EM_PLAY_MODE_SEQUENCE,      // 顺序播放
	EM_PLAY_MODE_RANDOM,        // 随机播放
	EM_PLAY_MODE_SINGLE_CIRCLE  // 单曲循环
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

	bool    OnPosChanged(void* param);                              // 进度改变，用户主动改变进度
	bool    OnVolumeChanged(void* param);                           // 音量改变

	 TStreamTime GetMusicLenght(CDuiString &MusicPath);
	 MEDIA* GetMusicID3Info(CDuiString &MusicPath);


	//歌曲播放回调函数
	static void player_proc(void *user_data,PLAY_MSG msg,LPARAM lParam);
	static CDuiString TimeToString(int time);

	void PaintAlbum(HDC hDC);
	void SendPlayState();
	void PlayNext();
	void PlayPrev(); 
	void Play(LPCTSTR pszPath);   // 播放路径为pszPath的文件
	void Play();          // 播放
	void Play(bool bPlay);	//播放或暂停	
	void Stop();         // 停止
	void OpenFileDialog();                                          // 打开文件窗口
	//void ShowPlaylist(bool bShow);                                  // 显示播放列表
	void AddFiles(vector<CDuiString> &fileNames, bool bInit = false);    // 添加文件到播放列表
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
	
	CDuiString      m_strPath;          // 当前文件的路径
	bool			IsClose;
	bool			IsReadyPlay;
	bool			m_bPlayState;		//播放状态标志位，true为在播放 false 暂停
	bool			m_bSwitch;			//手动切换标志位-双击播放时置为true

	ZPlay			*m_pZplayer;		 //播放器指针,用来控制播放
	ZPlay			*m_pSongInfo;		 //播放器指针,读取专辑信息

	MEDIA			*m_pCurMedia;		 //当前播放文件的信息
	CSliderUI       *m_pSliderPlay;     // 文件播放进度
	CTextUI         *m_pTextTime;		// 文件播放时间
	CTextUI         *m_pTextSongName;   // 正在播放的文件名称
	CButtonUI		*pbtnPlay;			//播放按钮-播放 
	CButtonUI		*pbtnPause;			//播放按钮-暂停
	CButtonUI		*pbtnTrayPlay;		//播放按钮-播放 任务栏
	CButtonUI		*pbtnTrayPause;		//播放按钮-暂停 任务栏

	CMusicListElementUI *m_pCurSong;	//当前播放的音乐文件
	CListUI			*m_pCurMusicList;	//当前播放的音乐列表

	CControlUI		*m_pDefaultAlbum;	//默认专辑封面
	CAlbumUI		*m_pCurAlbum;		//当前专辑封面

	enum_PlayMode   m_emPlayMode;       // 播放模式

	std::deque<unsigned>   m_queRand;   // 随机播放模式的文件序号


	void ShowPlayButton(bool bShow);                                // 显示播放按钮
	void ShowControlsForPlay(bool bShow);                           // 当开始播放或停止时，显示或隐藏一些控件


	HWND	mhMenuWnd;
	//CMenuWnd *m_pMainMenu;  ///设置菜单
};

extern HINSTANCE g_hPcmtInstance;