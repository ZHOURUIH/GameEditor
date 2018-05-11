#include <windows.h>

#include "EditorFrame.h"
#include "EditorApp.h"
#include "MemeryViewer.h"
#include "MemeryDetailWindow.h"
#include "MemeryTypeStatisticsWindow.h"

enum
{
	ID_TIMER,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mMemeryViewer(NULL),
mTimer(NULL),
mMemeryDetailWindow(NULL),
mMemeryTypeStatisticsWindow(NULL)
{
	;
}

EditorFrame::~EditorFrame()
{
	;
}

void EditorFrame::init()
{
	setup();
	mTimer = new wxTimer();
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	if (mTimer != NULL)
	{
		delete mTimer;
		mTimer = NULL;
	}
	mAuiManager.UnInit();
	if (mMemeryDetailWindow != NULL)
	{
		delete mMemeryDetailWindow;
		mMemeryDetailWindow = NULL;
	}
	if (mMemeryTypeStatisticsWindow != NULL)
	{
		delete mMemeryTypeStatisticsWindow;
		mMemeryTypeStatisticsWindow = NULL;
	}
	if (mMemeryViewer != NULL)
	{
		delete mMemeryViewer;
		mMemeryViewer = NULL;
	}
}

void EditorFrame::setup()
{
	// 创建编辑核心
	CreateEditorCore();

	mAuiManager.SetManagedWindow(this);
	// 创建菜单栏
	CreateMenu();
	// 创建工具栏
	CreateToolBar();
	// 创建各个子窗口
	CreateWindows();
	// 创建底部的状态显示栏
	CreateStatuBar();

	// 创建完后刷新一遍全部控件的选中状态
	RefreshAllMenuToolCheckState();

	// 刷新全部资源列表
	RefreshAllResource();
}

void EditorFrame::CreateMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// 文件菜单
	mFileMenu = new wxMenu();
	menuBar->Append(mFileMenu, "文件");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMemeryDetailWindow = new MemeryDetailWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mMemeryDetailWindow->init(mMemeryViewer);
	mAuiManager.AddPane(mMemeryDetailWindow, wxAuiPaneInfo().Name("MemeryDetailWindow").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("内存详细信息").Left().Dockable(true));
	mMemeryDetailWindow->Show();

	mMemeryTypeStatisticsWindow = new MemeryTypeStatisticsWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX, wxSize(700, -1));
	mMemeryTypeStatisticsWindow->init(mMemeryViewer);
	mAuiManager.AddPane(mMemeryTypeStatisticsWindow, wxAuiPaneInfo().Name("MemeryTypeStatisticsWindow").FloatingSize(500, 600).Caption("内存类型统计信息").Right().Dockable(true));
	mMemeryTypeStatisticsWindow->Show();

	mAuiManager.Update();
}

void EditorFrame::CreateStatuBar()
{
	CreateStatusBar(2);
	SetStatusText("camera pos : 0.0, 0.0, 0.0", 0);
	SetStatusText("camera dir : 0.0, 0.0, 0.0", 0);
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mMemeryViewer = new MemeryViewer();
}

void EditorFrame::RefreshAllResource()
{
	;
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	;
}

void EditorFrame::OnTimer(wxTimerEvent& event)
{
	static unsigned long lastTime = timeGetTime();
	unsigned long curTime = timeGetTime();
	float elapsedTime = (curTime - lastTime) / 1000.0f;
	Update(elapsedTime);
	lastTime = curTime;
	Render();
}

void EditorFrame::Update(float elapsedTime)
{
	if (mMemeryDetailWindow != NULL)
	{
		mMemeryDetailWindow->update(elapsedTime);
	}
	if (mMemeryTypeStatisticsWindow != NULL)
	{
		mMemeryTypeStatisticsWindow->update(elapsedTime);
	}
	KeyProcess();
	UpdateStatus();
}

void EditorFrame::Render()
{
	;
}

void EditorFrame::KeyProcess()
{
	;
}

void EditorFrame::OnExit(wxCommandEvent& event)
{
	// 发出关闭窗口的事件
	Close(true);
}

void EditorFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	// 销毁自己的数据
	destroy();
	// 销毁窗口
	Destroy();
}