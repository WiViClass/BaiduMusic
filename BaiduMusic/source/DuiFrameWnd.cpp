#include "..\include\DuiFrameWnd.h"
#include "..\include\resource.h"
#include <windowsx.h>
#include <algorithm>
#include <time.h>
#include <comutil.h>
#include "..\include\MenuWnd.h"


#pragma comment(lib, "comsupp.lib")
#pragma comment( lib, "libzplay.lib" )  


using namespace libZPlay;

const TCHAR STR_PATH_PLAYLIST[] = _T("Playlist.m3u");
const TCHAR STR_PATH_CONFIG[]   = _T("XMP.cfg");

// �ļ�ɸѡ
const TCHAR STR_FILE_FILTER[] =
	_T("All Files(*.*)\0*.*\0")
	_T("Music Files(*.mp3,*.wma,etc)\0*.mp3;*.wma;*.wav;*.mid;*.rmi;*.aac;*.ac3;*.aiff;*.amr;*.m4a;*.mka;*.mp2;*.ogg;*.ra;*.au;*.flac;\0");
const TCHAR STR_FILE_ALL[] = 
	_T("Music Files('\0')");

// �ļ����� 
// ����STR_FILE_FILTER��\0�����Բ������ڲ��ң������\0����|

const TCHAR STR_FILE_MUSIC[] =
	_T("Music Files(*.mp3,*.wma,etc)|*.mp3;*.wma;*.wav;*.mid;*.rmi;*.aac;*.ac3;*.aiff;*.amr;*.m4a;*.mka;*.mp2;*.ogg;*.ra;*.au;*.flac;|");


int __stdcall CallbackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned  param1, unsigned  param2);


bool FindFileExt(LPCTSTR pstrPath, LPCTSTR pstrExtFilter)
{
	if (! pstrPath || ! pstrExtFilter)
	{
		return false;
	}

	TCHAR szExt[_MAX_EXT] = _T("");

	_tsplitpath_s(pstrPath, NULL, 0, NULL, 0, NULL, 0, szExt, _MAX_EXT);
	_tcslwr_s(szExt, _MAX_EXT);

	if(_tcslen(szExt))  
	{
		_tcscat_s(szExt, _MAX_EXT, _T(";"));    // .mo���������������ڻ�ƥ�䵽.mov�������ں�����ϡ�;�����ж��Ƿ���ȫƥ��
		return NULL != _tcsstr(pstrExtFilter, szExt);
	}



	return false;
}

bool IsMusicFile(LPCTSTR pstrPath)
{
	return FindFileExt(pstrPath, STR_FILE_MUSIC);
}

bool IsWantedFile(LPCTSTR pstrPath)
{
	return IsMusicFile(pstrPath) ;
}

void EnumerateFiles(std::vector<CDuiString> &vctString)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(_T("*.*"), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do 
		{
			// ���ΪĿ¼
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(fd.cFileName, _T(".")) && _tcscmp(fd.cFileName, _T("..")))
				{
					::SetCurrentDirectory(fd.cFileName);    
					EnumerateFiles(vctString);              
					::SetCurrentDirectory(_T(".."));        
				}
			}
			// ���Ϊ�ļ�
			else
			{
				CDuiString strDir;
				TCHAR      lpDir[MAX_PATH];

				::GetCurrentDirectory(MAX_PATH, lpDir);
				strDir = lpDir;
				if ( strDir.Right(1) != _T("\\") )
				{
					strDir += _T("\\");
				}
				strDir += fd.cFileName;

				vctString.push_back(strDir.GetData());
			}
		} while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
}

// ����uRandNum������ͬ�������������ӵ�queRandĩβ

void Rand(std::deque<unsigned int> &queRand, unsigned int uRandNum)
{
	if (uRandNum <= 0)
	{
		return;
	}

	unsigned uSizeOld = queRand.size();
	unsigned uSizeNew = uSizeOld + uRandNum;
	queRand.resize(uSizeNew);
	srand(unsigned(time(NULL)));

	for(unsigned i = uSizeOld; i < uSizeNew; i++)
	{
		queRand[i] = i;
	}

	for(unsigned i = uSizeOld; i < uSizeNew; i++)
	{
		std::swap(queRand[i], queRand[rand() % uSizeNew]);
	}
}

std::string UnicodeConvert(const std::wstring& strWide, UINT uCodePage)
{
	std::string strANSI;
	int iLen = ::WideCharToMultiByte(uCodePage, 0, strWide.c_str(), -1, NULL, 0, NULL, NULL);

	if (iLen > 1)
	{ 
		strANSI.resize(iLen-1);
		::WideCharToMultiByte(uCodePage, 0, strWide.c_str(), -1, &strANSI[0], iLen, NULL, NULL);
	}

	return strANSI;
}

std::string UnicodeToUTF8(const std::wstring& strWide)
{
	return UnicodeConvert(strWide, CP_UTF8);
}

std::string UnicodeToANSI(const std::wstring& strWide)
{
	return UnicodeConvert(strWide, CP_ACP);
}


CDuiFrameWnd::CDuiFrameWnd( LPCTSTR pszXMLName )
	: CXMLWnd(pszXMLName),
	//m_pSliderPlay(NULL),
	//m_MusicPlayer(this), 
	m_pZplayer(NULL),
	m_pSongInfo(NULL),
	m_pCurMedia(NULL),
	m_pCurSong(NULL),
	m_bSwitch(false),
	m_bPlayState(false),
	IsClose(false)
{

}

CDuiFrameWnd::~CDuiFrameWnd()
{

}

DUI_BEGIN_MESSAGE_MAP(CDuiFrameWnd, CNotifyPump)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	//DUI_ON_MSGTYPE(DUI_MSGTYPE_RBUTTONMENU,OnDoubleClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE,OnItemActivate)

	DUI_END_MESSAGE_MAP()

void CDuiFrameWnd::InitWindow()
{
	SetIcon(IDI_ICON1);
	CreateTrayIcon( IDI_ICON1 ); // ��������ͼ��

	// �������ÿؼ���Ϊ��Ա����
	CSliderUI* pSilderVol = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderVol")));
	m_pSliderPlay = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderPlay")));
	m_pTextTime  = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("text_play_time")));
	m_pTextSongName=  static_cast<CTextUI*>(m_PaintManager.FindControl(_T("playing_song_name")));

	m_pDefaultAlbum=  static_cast<CControlUI*>(m_PaintManager.FindControl(_T("default_album_logo")));
	m_pCurAlbum=  static_cast<CAlbumUI*>(m_PaintManager.FindControl(_T("current_album_logo")));
	
	m_pCurMusicList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("list_music_file")));

	pbtnPlay  =static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_play")));

	pbtnPause = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btn_pause")));
	


	//m_MusicPlayer.InitPlayer(player_proc);``


	if ( ! m_pSliderPlay || ! m_pTextTime)
	{
		return;
	}
	//��δ���������Դ
//	pSilderVol->OnNotify    += MakeDelegate(this, &CDuiFrameWnd::OnVolumeChanged);
	m_pSliderPlay->OnNotify += MakeDelegate(this, &CDuiFrameWnd::OnPosChanged);

	m_pZplayer = CreateZPlay();
	m_pSongInfo = CreateZPlay();
	// ���ò������ص�����
	m_pZplayer->SetCallbackFunc(CallbackFunc, 
		(TCallbackMessage)(MsgPlay|MsgStop|MsgStopAsync|MsgPlayAsync|MsgWaveBuffer), this);
	

}


bool CDuiFrameWnd::CreateTrayIcon( WORD nIDResource )
{
	bool bSuccess = false;
	HICON hIcon = LoadIcon( g_hPcmtInstance, MAKEINTRESOURCE( nIDResource ) );
	bSuccess = CTrayIcon::Instance().Create( m_hWnd, WM_MY_TRAY_NOTIFICATION, PRODUCT_TRAY, hIcon, IDI_ICON1 );

	CTrayIcon::Instance().SetIcon( hIcon );
	CTrayIcon::Instance().ShowIcon();
	//g_pMainLogic->SetTrayIconID( nIDResource );  // ��¼����ͼ���ID


	return bSuccess;
}

int __stdcall  CallbackFunc(void* instance, void *user_data, TCallbackMessage message, unsigned  param1, unsigned  param2)
{
	ZPlay *m_Instance = (ZPlay*) instance;
	CDuiFrameWnd *player = (CDuiFrameWnd*)user_data;
	if (message & MsgStopAsync )
	{
		CDuiFrameWnd::player_proc(user_data,MSG_STOP,NULL);
	}
	else if(message & MsgWaveBuffer)
	{
		UINT num = 0;

		TStreamTime ttm;
		m_Instance->GetPosition(&ttm);
		num = ttm.sec;

		CDuiFrameWnd::player_proc(user_data,MSG_PLAYING,num);

	}
	return 0;
}

void CDuiFrameWnd::player_proc(void *user_data,PLAY_MSG msg,LPARAM lParam)
{
	CDuiFrameWnd* pMainFrame = (CDuiFrameWnd*)user_data;
	MEDIA* media = pMainFrame->m_pCurMedia;
	if (pMainFrame == NULL )
		return;

	switch(msg)
	{
	case MSG_OPEN:
		{
			if(pMainFrame->m_bSwitch)
			{
				pMainFrame->m_bSwitch = false;
			}
			pMainFrame->m_pTextSongName->SetText(media->PlayTitle);
			pMainFrame->m_pSliderPlay->SetMinValue(0);
			pMainFrame->m_pSliderPlay->SetMaxValue(media->totalTime.sec);
			pMainFrame->m_pSliderPlay->SetValue(0);
		}
		break;
	case MSG_PLAYING:
		{
			CDuiString playTime = CDuiFrameWnd::TimeToString(lParam);
			pMainFrame->m_pTextTime->SetText(playTime.GetData());
			pMainFrame->m_pSliderPlay->SetValue((int)lParam);
		}
		break;
	case MSG_PLAY:
		{

		}
		break;
	case MSG_PAUSE:
		{

		}
		break;
	case MSG_RESUME:
		{

		}
		break;
	case MSG_STOP:
		{
			if(pMainFrame->IsClose)
				return;


			if(pMainFrame->m_bSwitch)
			{
				pMainFrame->m_bSwitch = false;
				break;
			}else
			{
				pMainFrame->PlayNext();
			}
		}
		break;
	case MSG_VOLUME:
		{
		}
		break;
	}

}


CDuiString CDuiFrameWnd::TimeToString(int time)
{
	CDuiString sTime,sMin,sSec;
	if (time/60<10)
	{
		sMin.Format(L"0%d",time/60);
	}else
	{
		sMin.Format(L"%d",time/60);
	}

	if (time%60<10)
	{
		sSec.Format(L"0%d",time%60);
	}else
	{
		sSec.Format(L"%d",time%60);
	}

	sTime.Append(sMin);
	sTime.Append(L":");
	sTime.Append(sSec);
	return sTime;
}

CControlUI* CDuiFrameWnd::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcsicmp(pstrClassName, _T("MyAlbum")) == 0)
	{
		//strXML = _T("Caption.xml");
		CAlbumUI *pAlbum = new CAlbumUI;  
		return pAlbum;
	}else if(_tcsicmp(pstrClassName, _T("MySongList")) == 0)
	{
		CPlaylist *pSongList = new CPlaylist;
		return pSongList;
	}else if(_tcsicmp(pstrClassName, _T("MusicTest")) == 0)
	{
		CMusicListElementUI *pSongList = new CMusicListElementUI;
		return pSongList;
	}
	return NULL;

}

void CDuiFrameWnd::OnClick( TNotifyUI& msg )
{
	CDuiString sCtrlName = msg.pSender->GetName();

	if( sCtrlName == _T("btn_close") ) 
	{
		CloseWnd();
	}
	else if( sCtrlName == _T("btn_play_prev") ) 
	{
		m_bSwitch = true;
		PlayPrev();
	}
	else if( sCtrlName == _T("btn_play_next") ) 
	{
		m_bSwitch = true;
		PlayNext();
	}
	else if( sCtrlName == _T("btn_play") ) 
	{
		Play(true);
		
	}
	else if( sCtrlName == _T("btn_pause")  ) 
	{
		m_bSwitch = true;
		Play(false);
	}
	else if( sCtrlName == _T("btn_tools") ) 
	{
		// Stop();
	}
	else if( sCtrlName == _T("add_local_song") ) 
	{
		OpenFileDialog(); 
	}  
	else if( sCtrlName == _T("default_avatar") ) 
	{
	
		//AddTest();
	}
	else if( sCtrlName == _T("play_all") ) 
	{

		//DelateTest();
	}
	else if( sCtrlName == _T("sort") ) 
	{
		//OpenFileDialog(); 
		//ElementTest();
	}
	else  if( sCtrlName == _T("btnVolume") ) 
	{
		
		m_PaintManager.FindControl(_T("btnVolumeZero"))->SetVisible(true);
		msg.pSender->SetVisible(false);
	}
	else if( sCtrlName == _T("btnVolumeZero") ) 
	{
		CSliderUI* pUI = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderVol")));
		//m_pPlayer.Volume(pUI->GetValue());
		m_PaintManager.FindControl(_T("btnVolume"))->SetVisible(true);
		msg.pSender->SetVisible(false);
	}else if( sCtrlName == _T("btn_setting") ) 
	{
		// Stop();
		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		CMenuWnd *pMenu = new CMenuWnd(_T("main_menu.xml"));

		pMenu->Init(*this, pt);
		pMenu->ShowWindow(TRUE);
	}

	__super::OnClick(msg);
}
void CDuiFrameWnd::OnDoubleClick(TNotifyUI& msg)
{

//Itemɾ������
	m_pCurMusicList->Remove(msg.pSender);

}

void CDuiFrameWnd::OnItemActivate(TNotifyUI& msg)
{
	if ( _tcsicmp( msg.pSender->GetClass(), _T("MusicListElementUI") ) == 0 )
	{
		m_pCurSong =  (CMusicListElementUI*)msg.pSender;	
		CPlaylist *m_pPlayList  = static_cast<CPlaylist*>(m_PaintManager.FindControl(_T("list_music_file")));

		m_pCurSong->SetActive(true);
		m_pCurSong->AnimatePlay(true);
		m_pPlayList->SetItemActive(m_pCurSong->GetIndex());

		m_pCurMedia = m_pCurSong->GetMedia();
		m_pCurMedia->PlayTitle.Format(_T("%s - %s"), m_pCurMedia->Title.GetData(),m_pCurMedia->Artist.GetData());

		m_strPath = m_pCurMedia->pathFileName;
		m_bSwitch = true;
		
		Play();
	}
}

void CDuiFrameWnd::Notify( TNotifyUI& msg )
{
	if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		CDuiString    strName = msg.pSender->GetName();
		CTabLayoutUI* pTab    = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabTest")));
		if(strName == _T("my_music_tab"))
			pTab->SelectItem(0);
		else if(strName == _T("online_music_tab"))
			pTab->SelectItem(1);
		else if(strName == _T("my_device_tab"))
			pTab->SelectItem(2);
	}
	__super::Notify(msg);
}

LRESULT CDuiFrameWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);

	switch (uMsg)
	{
		HANDLE_MSG (*this, WM_DROPFILES, OnDropFiles);
	case WM_MY_TRAY_NOTIFICATION:
		{
			IsReadyPlay = true;
			CTrayIcon::Instance().OnIconNotification( wParam, lParam,IsReadyPlay );
		}
		return true;
	case WM_MY_MUSIC_PLAY:
		{
			m_bSwitch = true;
			Play(true);
			return true;
		}
	case WM_MY_MUSIC_PAUSE:
		{
			m_bSwitch = true;
			Play(false);
			return true;
		}
	case WM_MY_MUSIC_NEXT:
		{
			m_bSwitch = true;
			PlayNext();
			SendPlayState();
			return true;
		}
	case WM_MY_MUSIC_PREV:
		{
			m_bSwitch = true;
			PlayPrev();
			SendPlayState();
			return true;
		}
	case WM_MY_TRAY_SHOW_MENU:
		{
			mhMenuWnd = (HWND)wParam;
			 SendPlayState();
			return true;
		}
	}

	return lRes;
}

LRESULT CDuiFrameWnd::OnNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return __super::OnNcHitTest(uMsg, wParam, lParam, bHandled);
}

LRESULT CDuiFrameWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	return __super::ResponseDefaultKeyEvent(wParam);
}





void CDuiFrameWnd::OnDropFiles( HWND hwnd, HDROP hDropInfo )
{
	UINT  nFileCount = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = _T("");
	DWORD dwAttribute;
	std::vector<CDuiString> vctString;

	// ��ȡ��ק�����ļ����ļ���
	for (UINT i = 0; i < nFileCount; i++)
	{
		::DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		dwAttribute = ::GetFileAttributes(szFileName);

		// �Ƿ�Ϊ�ļ���
		if ( dwAttribute & FILE_ATTRIBUTE_DIRECTORY )
		{          
			::SetCurrentDirectory(szFileName);       
			EnumerateFiles(vctString);	      
		}
		else
		{
			vctString.push_back(szFileName);
		}
	}

	AddFiles(vctString);
	::DragFinish(hDropInfo);
}


void CDuiFrameWnd::OpenFileDialog()
{
	OPENFILENAME ofn;      
	
	TCHAR szOpenFileNames[80*MAX_PATH] ;  
	TCHAR szPath[MAX_PATH]  ;  
	
	std::vector<CDuiString> vctString;
	
	TCHAR* p_FileName;  
	int nLen = 0;  

	ZeroMemory(&ofn, sizeof(ofn));  
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner   = *this;
	ofn.lpstrFile   = szOpenFileNames;   
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile    = sizeof(szOpenFileNames);  
	ofn.lpstrFilter = STR_FILE_FILTER;

	ofn.nFilterIndex    = 2;  
	ofn.lpstrFileTitle  = NULL;  
	ofn.nMaxFileTitle   = 0;  
	ofn.lpstrInitialDir = NULL;  
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;  

	// ���ڴ򿪶Ի���Ϊ��ѡ���ʱ������OFN_ALLOWMULTISELECT
	// �رմ򿪶Ի�������ѡ�Ľ��������ofn.lpstrFile�У���'\0'�ָ�

	if( GetOpenFileName( &ofn ) )  
	{    
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:  
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'  
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�  
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );  

		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.  
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������  

		szPath[ ofn.nFileOffset ] = '/0';  
		nLen = lstrlen(szPath);  //���㳤��

		if( szPath[nLen-1] != '//' )   //���ѡ�˶���ļ�,��������'//'  
		{  
			lstrcat(szPath, TEXT("//"));  
		}  


		p_FileName = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�  

		while( *p_FileName )  
		{     	
			vctString.push_back(p_FileName);//��ָ����ӵ�������
			p_FileName += lstrlen(p_FileName) +1;     //������һ���ļ�  
		
		}  

		AddFiles(vctString);
	}  
}


void CDuiFrameWnd::ShowPlayButton( bool bShow )
{

	if (pbtnPlay && pbtnPause)
	{
		pbtnPlay->SetVisible(bShow);
		pbtnPause->SetVisible(! bShow);
	}
}


void CDuiFrameWnd::ShowControlsForPlay( bool bShow )
{
	ShowPlayButton(!bShow);
}

void CDuiFrameWnd::AddFiles( vector<CDuiString> &fileNames, bool bInit )
{

	CDuiString str;
	CDuiString str_Title;
	//	CDuiString str_Title;
	CDuiString str_Artist;
	CDuiString str_Album;
	CPlaylist* pList = static_cast<CPlaylist*>(m_PaintManager.FindControl(_T("list_music_file")));
	CDuiString pathFileName;

	CVerticalLayoutUI* pVertical = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("empty_song_list")));
	pVertical->SetVisible(false);

	TCHAR       szName[_MAX_FNAME];
	TCHAR       szExt[_MAX_EXT];
	unsigned    i, uWantedCount;

	for(i = 0, uWantedCount = 0; i < fileNames.size(); i++)
	{
		if (IsWantedFile(fileNames[i].GetData()))
		{
			_tsplitpath_s(fileNames[i].GetData(), NULL, 0, NULL, 0, szName, _MAX_FNAME, szExt, _MAX_EXT);
			str.Format(_T("%s"), szName);   // �ļ���


			//	GetStreamInfo(&pInfo);


			// ���List�б����ݣ�������Add(pListElement)����SetText
			CMusicListElementUI* pListElement = new CMusicListElementUI;
			//pListElement->m_pMedia = new MEDIA;
			//pListElement->m_pMedia=GetMusicID3Info(fileNames[i]);
			//pMedia=
			pList->Add(pListElement);
			//pListElement->SetTextPadding(CDuiRect(25,0,0,0));

			pList->SetItemTextPadding(CDuiRect(22,5,0,5));
			pListElement->CElementSetValue(GetMusicID3Info(fileNames[i]));


			uWantedCount++;

			if (! bInit)
			{
				//m_cPlayList.Add(fileNames[i]);          // ����·��
			}
		}        
	}

	Rand(m_queRand, uWantedCount);




	// ShowPlaylist(true);
}


TStreamTime CDuiFrameWnd::GetMusicLenght(CDuiString &MusicPath)
{
	TStreamInfo pInfo = {0};
	if (m_pZplayer->OpenFileW(MusicPath.GetData(), sfAutodetect))
		m_pZplayer->GetStreamInfo(&pInfo);

	return pInfo.Length;
}


MEDIA* CDuiFrameWnd::GetMusicID3Info(CDuiString &MusicPath)
{
	TID3InfoExW pID3Info = {0};
	TStreamInfo pInfo = {0};
	MEDIA* pMedia = new MEDIA;
	//m_pSongInfoר�����ڻ�ȡ�ļ���Ϣ��ֹ�����ж�
	if (m_pSongInfo->OpenFileW(MusicPath.GetData(), sfAutodetect))
	{
		m_pSongInfo->GetStreamInfo(&pInfo);

		if(!m_pSongInfo->LoadID3ExW(&pID3Info, 1))
			memset(&pID3Info,0 , sizeof(TID3InfoExW));
	}


	// HBITMAP hBmp = CreateCompatibleBitmap(hdc,nWidth,nHeight);  

	pMedia->Title	  = pID3Info.Title;
	pMedia->Album	  = pID3Info.Album;
	pMedia->Artist	  = pID3Info.Artist;
	pMedia->totalTime = pInfo.Length;
	pMedia->pathFileName =  MusicPath;

	pMedia->StringTime =CDuiFrameWnd::TimeToString(pMedia->totalTime.sec);

	return pMedia;
}


void CDuiFrameWnd::PaintAlbum(HDC hDC)
{

	//m_pCurAlbum->DoPaint(hDC,m_pCurAlbum->GetPos());

	//C* pUI = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("sliderVol")));
}

void CDuiFrameWnd::AddTest()
{

	//list���ܲ���
	CVerticalLayoutUI* pVertical = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("empty_song_list")));
	pVertical->SetVisible(false);

	CDuiString str;
	CPlaylist* pList = static_cast<CPlaylist*>(m_PaintManager.FindControl(_T("list_music_file")));

	// ���List�б����ݣ�������Add(pListElement)����SetText
	int jk=0;

	//pListElement->SetTextPadding(CDuiRect(30, 0, 0, 0));
	for(int i=0; i<3; i++)

	{
		CMusicListElementUI* pListElement = new CMusicListElementUI;

		pListElement->SetTag(i);
		pList->Add(pListElement);

		//str.Format(_T("%d"), i);
		//pListElement->SetText(0, _T("000000123456789"));
		//pListElement->SetText(1, _T("JOY"));
		//pListElement->SetText(2, _T("hehe"));
		//pListElement->SetText(3, _T("000000123456789"));
		//pListElement->SetText(4, _T("000000123456789"));
	}

}

void CDuiFrameWnd::ElementTest()
{

	//list���ܲ���
	CVerticalLayoutUI* pVertical = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("empty_song_list")));
	pVertical->SetVisible(false);

	CDuiString str;
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("list_music_file")));

	// ���List�б����ݣ�������Add(pListElement)����SetText
	int jk=0;

	//pListElement->SetTextPadding(CDuiRect(30, 0, 0, 0));
	for(int i=0; i<2; i++)

	{
		CMusicListElementUI* pListElement = new CMusicListElementUI;

		pListElement->SetTag(i);
		pList->Add(pListElement);
		//pListElement->SetFixedHeight(30);
		str.Format(_T("%d"), i);
		//pListElement->SetText(0, str);
		//pListElement->SetText(1, _T("JOY"));
		//pListElement->SetText(2, _T("hehe"));
		//pListElement->SetText( _T("000000123456789"));
		//pListElement->SetText(4, _T("000000123456789"));
	}

}

void CDuiFrameWnd::DelateTest()
{

	//list���ܲ���


	CDuiString str;
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("list_music_file")));

	pList->RemoveAll();



	//// ���List�б����ݣ�������Add(pListElement)����SetText
	//int jk=0;
	//CMusicListElementUI* pListElement = new CMusicListElementUI;

	////pListElement->SetTextPadding(CDuiRect(30, 0, 0, 0));
	////pListElement->SetTag(jk);
	//pList->Add(pListElement);
	////pListElement->SetFixedHeight(30);
	//str.Format(_T("%d"), jk);
	//pListElement->SetText(0, str);
	//pListElement->SetText(1, _T("JOY"));
	//pListElement->SetText(2, _T("hehe"));

}


void CDuiFrameWnd::SendPlayState()
{
	if(m_pCurMedia)
	::SendMessage(mhMenuWnd, WM_MY_MUSIC_STATE, (WPARAM) m_bPlayState, (LPARAM) (m_pCurMedia->Title.GetData()));
}

void CDuiFrameWnd::PlayPrev()
{
	if(m_pCurMusicList->GetCount() == 0)
	{
		return ;
	}
	int CurSelect;
	if(m_pCurSong != NULL)
	{
		CurSelect = m_pCurSong->GetIndex()-1;
	}
	else
	{
		CurSelect = m_pCurMusicList->GetCurSel()-1;
	}

	if(CurSelect < 0)
		CurSelect = 0;

	m_pCurSong = static_cast<CMusicListElementUI*>(m_pCurMusicList->GetItemAt(CurSelect));

	CPlaylist *m_pPlayList  = static_cast<CPlaylist*>(m_PaintManager.FindControl(_T("list_music_file")));
	m_pCurSong->SetActive(true);
	m_pPlayList->SetItemActive(CurSelect);

	m_pCurMedia = m_pCurSong->GetMedia();
	m_pCurMedia->PlayTitle.Format(_T("%s - %s"), m_pCurMedia->Title.GetData(),m_pCurMedia->Artist.GetData());
	m_strPath = m_pCurMedia->pathFileName;
	Play();
}


void CDuiFrameWnd::PlayNext()
{
	int CurSelect;
	int TotalCount = m_pCurMusicList->GetCount();
	if(TotalCount == 0)
	{
		return ;
	}

	if(m_pCurSong != NULL)
	{
		CurSelect = m_pCurSong->GetIndex()+1;
	}
	else
	{
		CurSelect = m_pCurMusicList->GetCurSel();
		if(CurSelect<0)
			CurSelect=0;
	}

	if(CurSelect > TotalCount-1)
	{
		CurSelect =  TotalCount-1;
	}

	m_pCurSong = static_cast<CMusicListElementUI*>(m_pCurMusicList->GetItemAt(CurSelect));

	CPlaylist *m_pPlayList  = static_cast<CPlaylist*>(m_PaintManager.FindControl(_T("list_music_file")));
	m_pCurSong->SetActive(true);
	m_pPlayList->SetItemActive(CurSelect);

	m_pCurMedia = m_pCurSong->GetMedia();
	m_pCurMedia->PlayTitle.Format(_T("%s - %s"), m_pCurMedia->Title.GetData(),m_pCurMedia->Artist.GetData());
	m_strPath = m_pCurMedia->pathFileName;

	Play();
}


void CDuiFrameWnd::Play()
{
	if (m_pCurMedia == NULL || m_pCurMedia->pathFileName == _T(""))
		return;
	//if (GetPlayState() != P_STOP)
	//{
	//	m_pZplayer->Stop();
	//}

	if(m_pCurSong == NULL)
	{
		m_pCurSong = static_cast<CMusicListElementUI*>(m_pCurMusicList->GetItemAt(m_pCurMusicList->GetCurSel()));
	}

	if (m_pZplayer->OpenFileW(m_strPath.GetData(), sfAutodetect))
	{
	
		player_proc(this,MSG_OPEN,NULL);

		TID3InfoEx id3_info = {0};
		if(m_pZplayer->LoadID3Ex(&id3_info, 1))
		{

			m_pCurAlbum->SetPicture(id3_info.Picture);
			m_pCurAlbum->Invalidate();
		//	int nWidth = id3_info.Picture.Width;  
		//	int nHeight = id3_info.Picture.Height;  

		//	HDC  hdc = GetDC(m_hWnd);  
		//	HDC myDC = CreateCompatibleDC(hdc);  
		//	HBITMAP hBmp = CreateCompatibleBitmap(hdc,nWidth,nHeight);  
		//	SelectObject(myDC,hBmp); 

		//	if(id3_info.Picture.CanDrawPicture)			
		//	{
		//		if(!m_pCurAlbum->IsVisible())	
		//		m_pCurAlbum->SetVisible(true);

		//			m_pZplayer->DrawBitmapToHDC(myDC, 0, 0, 0, 0, id3_info.Picture.hBitmap);
		//			m_pCurAlbum->SetHBitMap(hBmp);
		//			m_pCurAlbum->Invalidate();
		//			m_pDefaultAlbum->SetVisible(false);
		//	}
		//	else
		//	{
		//		// can't decode attached image to hBitmap, use PictureData and PictureDataSize
		//		// to decode this format by your own image decoder
		//		if(m_pCurAlbum->IsVisible())	
		//		m_pCurAlbum->SetVisible(false);
		//		m_pDefaultAlbum->SetVisible(true);

		//	}

		//	//if(hBmp)
		////	m_pDefaultAlbum->SetVisible(false);

		//	DeleteDC(myDC);
		}

	}else
	{
		m_pZplayer->Close();
		ShowControlsForPlay(false);
		return;
	}
	if (m_pZplayer->Play() == 0)
	{
		m_pZplayer->Close();
		ShowControlsForPlay(false);
	}
	m_pCurSong->AnimatePlay(true);
	ShowControlsForPlay(true);
	m_bPlayState = true;
	player_proc(this,MSG_PLAY,NULL);
}

void CDuiFrameWnd::Play(bool bPlay)
{
	m_bPlayState = bPlay;

	
	if(bPlay)
	{
		int playProcess = m_pSliderPlay->GetValue();
		if (m_pCurMedia == NULL || m_pCurMedia->pathFileName == _T(""))
		{
			PlayNext();
			return;
		}
		m_pCurSong->AnimatePlay(bPlay);
		if(playProcess)
		{
			TStreamTime ttm;
			ttm.ms = playProcess * 1000;  //������Ҫ���ú��룬����Seek������ʧЧ
			m_pZplayer->Seek(tfMillisecond,&ttm,smFromBeginning);
			ShowControlsForPlay(true);
		}else
		{
			Play();
		}
	}else
	{
		m_pZplayer->Pause();
		m_pCurSong->AnimatePlay(bPlay);
		ShowControlsForPlay(false);

	}

}

void CDuiFrameWnd::Play( LPCTSTR pszPath )
{
	if (! pszPath)
	{
		return;
	}
	m_strPath = pszPath;
	ShowControlsForPlay(true);
}

void CDuiFrameWnd::Stop()
{
    m_pZplayer->Stop();
	ShowControlsForPlay(false);
}

bool CDuiFrameWnd::OnPosChanged( void* param )
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	if( pMsg->sType == _T("valuechanged") )
	{
		int playProcess = static_cast<CSliderUI*>(pMsg->pSender)->GetValue();

		if (m_pCurMedia == NULL || m_pCurMedia->pathFileName == _T(""))
			return false;

		TStreamTime ttm;
		ttm.ms = playProcess * 1000;  //������Ҫ���ú��룬����Seek������ʧЧ
		m_pZplayer->Seek(tfMillisecond,&ttm,smFromBeginning);
	}
	return true;
}

bool CDuiFrameWnd::OnVolumeChanged( void* param )
{
	TNotifyUI* pMsg = (TNotifyUI*)param;

	if( pMsg->sType == _T("valuechanged") )
	{
		//m_pPlayer.Volume((static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
	}

	return true;
}

void CDuiFrameWnd::CloseWnd()
{
	IsClose =  true;
	m_pZplayer->Close();
	m_pZplayer->Release();
	m_pSongInfo->Release();
	Close();
}
