#include "EditorFrame.h"
#include "EditorApp.h"
#include "MainViewerPanel.h"
#include "HistoryDatePanel.h"
#include "DataBase.h"

enum
{
	ID_TIMER,
	ID_TOOL_HISTROY_DATE_PANEL,
	ID_TOOL_MAIN_VIEWER_PANEL,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_TOOL(ID_TOOL_HISTROY_DATE_PANEL, OnPanelShow)
EVT_TOOL(ID_TOOL_MAIN_VIEWER_PANEL, OnPanelShow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mTimer(NULL),
mDataBase(NULL),
mAppNormalName(title)
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
	TRACE_DELETE(mDataBase);
	TRACE_DELETE(mTimer);
	mAuiManager.UnInit();
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
	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_HISTROY_DATE_PANEL, wxT("日期窗口"), wxBITMAP(IDB_PANEL_DATE), wxT("显示日期窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_MAIN_VIEWER_PANEL, wxT("数据窗口"), wxBITMAP(IDB_PANEL_INFO), wxT("显示数据窗口"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("窗口工具栏")).ToolbarPane().Top());
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainViewerPanel = new MainViewerPanel(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mAuiManager.AddPane(mMainViewerPanel, wxAuiPaneInfo().Name("MainViewerPanel").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("数据内容").Center());
	mMainViewerPanel->Show();
	mWindowIDList.insert(std::make_pair(mMainViewerPanel, ID_TOOL_MAIN_VIEWER_PANEL));

	mHistoryDatePanel = new HistoryDatePanel(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE);
	mAuiManager.AddPane(mHistoryDatePanel, wxAuiPaneInfo().Name("HistoryDatePanel").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("历史日期").Left());
	mHistoryDatePanel->Show();
	mWindowIDList.insert(std::make_pair(mHistoryDatePanel, ID_TOOL_HISTROY_DATE_PANEL));

	mAuiManager.Update();
}

void EditorFrame::CreateStatuBar()
{
	std::map<wxWindow*, int>::iterator iterWindow = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterWindowEnd = mWindowIDList.end();
	for (; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowToolBar->ToggleTool(iterWindow->second, check);
	}
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mDataBase = TRACE_NEW(DataBase, mDataBase);
	mDataBase->init(true);
}

void EditorFrame::RefreshAllResource()
{
	mHistoryDatePanel->showHistoryDate();
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

void EditorFrame::errorMessage(const std::string& str)
{
	wxMessageBox(str);
}

void EditorFrame::notifyWindowShow(wxWindow* window, const bool& show)
{
	std::map<wxWindow*, int>::const_iterator iterWindow = mWindowIDList.find(window);
	if (iterWindow == mWindowIDList.end())
	{
		return;
	}
	mWindowToolBar->ToggleTool(iterWindow->second, show);
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

void EditorFrame::OnPanelShow(wxCommandEvent& event)
{
	int eventID = event.GetId();
	std::map<wxWindow*, int>::iterator iterPanel = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterPanelEnd = mWindowIDList.end();
	for (; iterPanel != iterPanelEnd; ++iterPanel)
	{
		if (iterPanel->second == eventID)
		{
			if (mAuiManager.GetPane(iterPanel->first).IsShown() != event.IsChecked())
			{
				mAuiManager.GetPane(iterPanel->first).Show(event.IsChecked());
				mAuiManager.Update();
			}
			break;
		}
	}
}