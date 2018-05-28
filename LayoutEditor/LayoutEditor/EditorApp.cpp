#include "txEngineDefine.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "RenderCanvas.h"
#include "EditorBusyInfo.h"

#pragma comment( lib, "Dbghelp.lib" )

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

void EditorApp::OnKeyDown(wxKeyEvent& event)
{
	if (mMainFrame != NULL && mMainFrame->getRenderCanvas() != NULL)
	{
		mMainFrame->getRenderCanvas()->OnKeyDown(event);
	}
	event.Skip();
}

void EditorApp::OnKeyUp(wxKeyEvent& event)
{
	if (mMainFrame != NULL && mMainFrame->getRenderCanvas() != NULL)
	{
		mMainFrame->getRenderCanvas()->OnKeyUp(event);
	}
	event.Skip();
}

void EditorApp::OnMouseWheel(wxMouseEvent& event)
{
	if (mMainFrame != NULL && mMainFrame->getRenderCanvas() != NULL)
	{
		mMainFrame->getRenderCanvas()->OnMouseWheel(event);
	}
}

EditorBusyInfo* EditorApp::getBusyInfo()
{
	return mBusyInfo;
}

void EditorApp::destroyBusyInfo()
{
	TRACE_DELETE(mBusyInfo);
}

LONG EditorApp::ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char fileName[1024];
	SPRINTF(fileName, 1024, "%d-%d-%d %d时%d分%d秒.dmp", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	HANDLE hDumpFile = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);

	// 这里弹出一个错误对话框并退出程序  
	FatalAppExit(-1, _T("程序崩溃!!"));

	return EXCEPTION_EXECUTE_HANDLER;
}

bool EditorApp::OnInit()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
	mBusyInfo->show();
	mBusyInfo->setBusyInfo("正在创建编辑架构...");
	mMainFrame = new EditorFrame("LayoutEditor", wxSize(1440, 900));
	mMainFrame->init();
	mMainFrame->Show(true);
	mMainFrame->Maximize(true);
	mBusyInfo->hide();
	return true;
}