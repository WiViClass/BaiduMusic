#if 1
#pragma once

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

enum LOOP_STATE
{
	LOOP_SINGLE = 1,    //����ѭ��
	PLAY_SINGLE = 2,    //��������
	LOOP_LIST = 4,      //�б�ѭ��
	PLAY_LIST = 8,      //˳�򲥷�
	LOOP_RANDOM = 16,   //�������
	EXIT_APP = 32		//�����˳�
};

class CMusicListItemUI;
typedef struct tagMEDIA
{
    CDuiString  name;              //����
	CDuiString  pathFileName;      //�ļ�·����
	CDuiString  logoPath;		   //logoͼƬ��·��
	CDuiString	exName;			   //��չ��
	CDuiString  Title;
	CDuiString  Artist;
	CDuiString  Album;              //����

	UINT		playNum;		   //���Ŵ���
	CDuiString	size;			   //��С  (M)
	TStreamTime totalTime;         //��ʱ��	
	CMusicListItemUI* pControl;    //�󶨵Ŀؼ�ָ��
	CDuiString  tag;			   //�����ֶΣ����Դ�Ŷ�����Ϣ
	tagMEDIA():pControl(NULL)
	{
		ZeroMemory(&totalTime, sizeof(totalTime));
	}
}MEDIA;

enum PLAYER_STATE
{
	P_PLAY = 0,
	P_PAUSE,
	P_STOP
};

class CMusicPlayer;
class CDuiFrameWnd;
typedef void (*PLAY_PROC)(CMusicPlayer*,PLAY_MSG,WPARAM,LPARAM,void*);
typedef MEDIA* (*GET_MEDIA)(CMusicPlayer*);
unsigned __stdcall playing_proc( void* pArguments );
class CMusicPlayer
{
public:
	CMusicPlayer(CDuiFrameWnd *MainFrame);
	~CMusicPlayer(void);
	CDuiFrameWnd *GetMainFrame() const;

	static CDuiString TimeToString(int ms);
    TStreamTime GetMusicLenght();	 //��ȡ�����¼�����
	TID3InfoExW GetMusicID3Info();	 //�����ĵ���Ϣ�����ߣ����⣬���������·�ע��

	void InitPlayer(PLAY_PROC playProc);  //��ʼ��
	void OpenMedia(MEDIA* media, BOOL IsManual = false);    //��ý�����IsManual�ֶ�������ý�壿
	void Play();
	void Pause();
	void Resume();					  //����ͣ�ָ�
	void Stop();
	void Seek(int);					  //���ò���λ��
	void PlayNext();				  //������һ��
	void PlayPrev();				  //������һ��
	void PlayRandom();				  //�������

	void SetVolume(int volume);       //��������
	int  GetVolume();
	int  GetBitrate(int fAverage);		//��ȡ������
	void SetMixChannels(int fEnable);	//����ͨ�����
	int  GetMixChannels();
	void SetEcho(int fEnable);			//���û���
	int	 GetEcho();	
	void SetEqualizer(int fEnable);		//���þ�����
	int  GetEqualizer();
	void SetStereoCut(int fEnable);		//����ȥ������
	int  GetStereoCut();
	bool SetReverseMode(int fEnable);	//���õ��˲���
	int  GetReverseMode();
	void SetTempo(UINT nTempo);			//�����ٶ�
	UINT GetTempo();
	void SetPitch(UINT nPitch);			//��������
	UINT GetPitch();
	void SetRate(UINT nRate);			//��������
	UINT GetRate();	
	void SetLoopState(LOOP_STATE state);//����ѭ��״̬
	LOOP_STATE GetLoopState();
	PLAYER_STATE GetPlayState();		//��ȡ����״̬


	//UINT CMusicPlayer::MessageBox( LPCTSTR lpText, UINT uIcon = MB_ICONWARNING , LPCTSTR lpCaption = _T("��ʾ"),UINT uType = MB_OK );

	PLAY_PROC m_PlayProc;				//���Żص�����ָ��
	GET_MEDIA m_PlayNextMedia;			//��ȡ��һ��Media�Ļص�����
	GET_MEDIA m_PlayPrevMedia;			//��ȡ��һ��Media�ص�����
	GET_MEDIA m_PlayRandomMedia;			//��ȡ��ǰ�б����һ��Media�ص�����

	MEDIA *m_CurMedia;					//��ǰ�򿪵�Media
	ZPlay *m_pZplayer;					//������ָ��
	BOOL		m_bIsManualSwitch;		//�ֶ��л����֣���ִ��CallBack�е���ش���
private:
	LOOP_STATE m_CurLoopState;
	PLAYER_STATE m_PlayerState;
	TStreamStatus m_status;	
	int			m_volume;	

	CDuiFrameWnd	*m_pMainFrame;
};
#endif