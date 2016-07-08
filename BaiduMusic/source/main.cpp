
#include "..\include\DuiFrameWnd.h"
#include "..\include\KDetectMemoryLeak.h"

HINSTANCE g_hPcmtInstance = NULL;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	
	// �˴�����ǰӦ�ó����ʵ�����浽g_hInstance����Ϊ�е�API����ʱҪʹ�øò���
	g_hPcmtInstance = hInstance;

	//atexit(CheckOnExit);
	::CoInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("Res"));

	CDuiFrameWnd *pFrame = new CDuiFrameWnd(_T("main.xml"));
	pFrame->Create(NULL, _T("BaiduMusic"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	
	//CDuiFrameWnd::player_proc();
	pFrame->CenterWindow();
	pFrame->ShowModal();

	delete pFrame;
	::CoUninitialize();
    return 0;
}
