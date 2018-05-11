#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorBusyInfo.h"
#include "resource.h"
#include "FileUtility.h"
#include "StringUtility.h"
#include "txUtility.h"
#include "EditorCore.h"

IMPLEMENT_APP(EditorApp)

BEGIN_EVENT_TABLE(EditorApp, wxApp)
EVT_KEY_DOWN(OnKeyDown)
EVT_KEY_UP(OnKeyUp)
EVT_MOUSEWHEEL(OnMouseWheel)

END_EVENT_TABLE()


EditorApp::EditorApp()
:
mBusyInfo(NULL),
mMainFrame(NULL)
{
	;
}

EditorApp::~EditorApp()
{
	mMainFrame = NULL;
}

void EditorApp::destroyBusyInfo()
{
	TRACE_DELETE(mBusyInfo);
}

void EditorApp::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnKeyUp(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();
}

bool EditorApp::OnInit()
{
	std::string GAME_NAME = "dndl4.4.exe";
	HWND hwnd = GetForegroundWindow();//直接获得前景窗口的句柄         
	SendMessageA(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICON)));

	// 判断当前是否已经运行了该游戏,如果已经运行则直接退出
	txSet<std::string> processList;
	txUtility::getAllProcess(processList);  
	if (processList.find(GAME_NAME) != processList.end())
	{
		MessageBoxA(NULL, "游戏已经运行!", "提示", MB_OK);
		return false;
	}

	mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
	mBusyInfo->hide();
	wxSize size(600, 400);
	wxPoint pos((1920 - size.x) / 2, (1080 - size.x) / 2);
	mMainFrame = new EditorFrame(this, GAME_NAME, "版本检查", pos, size);
	mMainFrame->init();
	mMainFrame->Show(true);
	// 查找所有需要上传的文件
	mMainFrame->startCheckVersion();
	return true;
}