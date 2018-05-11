#include "EditorCore.h"
#include "EventSystem.h"
#include "txMemeryTrace.h"
#include "Utility.h"

#include "EditorFrame.h"
#include "EditorApp.h"
#include "EditorCommandHeader.h"
#include "EditorBase.h"
#include "DownloadPanel.h"
#include "EditorBusyInfo.h"
#include "OKDialog.h"

enum
{
	ID_TIMER,
	ID_MENU_EXIT,
	ID_TOOL_DOWNLOAD,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_EXIT, OnExit)

END_EVENT_TABLE()

EditorFrame::EditorFrame(EditorApp* app, const std::string& gameFullName, wxString title, wxPoint position, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, position, size),
txCommandReceiver("EditorFrame"),
mApp(app),
mTimer(NULL),
mFileMenu(NULL),
mEditorCore(NULL),
mDownloadPanel(NULL),
mRunning(true)
{}

EditorFrame::~EditorFrame()
{
	wxGetApp().destroyBusyInfo();
}

void EditorFrame::init()
{
	setup();
	mTimer = TRACE_NEW(wxTimer, mTimer);
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	TRACE_DELETE(mTimer);
	unregisterEditorCoreEventHandler();
	TRACE_DELETE(mEditorCore);
	mAuiManager.UnInit();
}

void EditorFrame::setup()
{
	mAuiManager.SetManagedWindow(this);
	// 创建编辑核心
	CreateEditorCore();
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
	mFileMenu->Append(ID_MENU_EXIT, wxT("退出"));
	menuBar->Append(mFileMenu, "文件");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	bool hasTool = false;
	if (hasTool)
	{
		mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
		mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
		mWindowToolBar->Realize();
		mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("窗口工具栏")).ToolbarPane().Top());
		mAuiManager.Update();
	}
}

#define NEW_PANEL(type, panel, title, toolID, size, floatingSize, dock)																										\
	panel = new type(this, wxDefaultPosition, size, wxMINIMIZE | wxMAXIMIZE, wxT(TOSTRING(type)));																			\
	mAuiManager.AddPane(panel, wxAuiPaneInfo().Name(panel->GetName()).BestSize(wxSize(-1, -1)).FloatingSize(floatingSize).Caption(wxT(title)).##dock##().Dockable(true));	\
	mWindowIDList.insert(std::make_pair(panel, toolID));																														\
	mWindowList.insert(std::make_pair(panel->GetName().ToStdString(), panel));

#define NEW_PANEL_LEFT(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(400, -1), wxSize(300, 800), Left)
#define NEW_PANEL_RIGHT(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(400, -1), wxSize(300, 800), Right)
#define NEW_PANEL_BOTTOM(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(-1, 400), wxSize(1200, 400), Bottom)
#define NEW_PANEL_CENTER(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(-1, -1), wxSize(-1, -1), Center)
#define NEW_PANEL_LEFT_FAST(className, toolID) NEW_PANEL_LEFT(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_RIGHT_FAST(className, toolID) NEW_PANEL_RIGHT(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_BUTTOM_FAST(className, toolID) NEW_PANEL_BOTTOM(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_CENTER_FAST(className, toolID) NEW_PANEL_CENTER(className, m##className, TOSTRING(className), toolID)

void EditorFrame::CreateWindows()
{
	// 在此处创建窗口
	NEW_PANEL_CENTER_FAST(DownloadPanel, ID_TOOL_DOWNLOAD);
	mAuiManager.Update();
	// 所有窗口都构造完成后通知EditorBase
	EditorBase::notifyConstructDone();
}

void EditorFrame::CreateStatuBar()
{
	;
}

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	mEditorCore = TRACE_NEW(EditorCore, mEditorCore);
	mEditorCore->init();
	mCommandSystem = mEditorCore->getCommandSystem();
	registerEditorCoreEventHandler();
	mEditorCore->notifyInitDone();
}

void EditorFrame::RefreshAllResource()
{
	;
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	std::map<wxWindow*, int>::iterator iterWindow = mWindowIDList.begin();
	std::map<wxWindow*, int>::iterator iterWindowEnd = mWindowIDList.end();
	for (; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		if (mWindowToolBar != NULL)
		{
			mWindowToolBar->ToggleTool(iterWindow->second, check);
		}
	}
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
	static bool frameDone = true;
	if (!frameDone)
	{
		return;
	}
	frameDone = false;
	if (!mRunning)
	{
		Close(true);
		return;
	}
	if (mEditorCore == NULL)
	{
		return;
	}
	mEditorCore->update(elapsedTime);
	KeyProcess();
	UpdateStatus();
	frameDone = true;
}

void EditorFrame::Render()
{
	;
}

void EditorFrame::KeyProcess()
{
	;
}

WXLRESULT EditorFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void EditorFrame::registerEditorCoreEventHandler()
{
	mEditorCore->getEventSystem()->registerAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEventHandler()
{
	mEditorCore->getEventSystem()->unregisterAllEvent(this);
}

void EditorFrame::onEditorCoreEvent(const CORE_EVENT_TYPE& type, const txVector<std::string>& params)
{
	if (type == CET_ERROR_LOG)
	{
		EditorUtility::logError(params[0]);
	}
	else if (type == CET_INFO_LOG)
	{
		EditorUtility::logInfo(params[0]);
	}
	else if (type == CET_DOWNLOAD_VERSION)
	{
		EditorBusyInfo* busyInfo = wxGetApp().getBusyInfo();
		if (!StringUtility::stringToBool(params[0]))
		{
			busyInfo->show();
			busyInfo->setBusyInfo((std::string("正在下载版本文件...")).c_str());
		}
		else
		{
			busyInfo->hide();
		}
	}
	else if (type == CET_DOWNLOAD_LIST_FILE)
	{
		EditorBusyInfo* busyInfo = wxGetApp().getBusyInfo();
		if (!StringUtility::stringToBool(params[0]))
		{
			busyInfo->show();
			busyInfo->setBusyInfo((std::string("正在下载列表文件...")).c_str());
		}
		else
		{
			busyInfo->hide();
		}
	}
	// 列表文件下载完毕,开始检查本地文件
	else if (type == CET_START_GENERATE_LOCAL_FILE)
	{
		EditorBusyInfo* busyInfo = wxGetApp().getBusyInfo();
		busyInfo->show();
		busyInfo->setBusyInfo((std::string("正在检查本地文件...0/") + params[0]).c_str());
	}
	// 正在检查本地文件
	else if (type == CET_GENERATING_LOCAL_FILE)
	{
		EditorBusyInfo* busyInfo = wxGetApp().getBusyInfo();
		busyInfo->setBusyInfo((std::string("正在检查本地文件...") + params[1] + "/" + params[0]).c_str());
	}
	// 文件对比完成
	else if (type == CET_FINISH_GENERATE_MODIFY_FILE)
	{
		EditorBusyInfo* busyInfo = wxGetApp().getBusyInfo();
		busyInfo->hide();
		mDownloadPanel->notifyFileList(params[0]);
	}
}

wxWindow* EditorFrame::getWindow(const std::string& name)
{
	std::map<std::string, wxWindow*>::iterator iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		return iter->second;
	}
	return NULL;
}

void EditorFrame::showPanel(wxWindow* panel, bool show)
{
	if (mAuiManager.GetPane(panel).IsShown() != show)
	{
		mAuiManager.GetPane(panel).Show(show);
		mAuiManager.Update();
	}
}

void EditorFrame::startCheckVersion()
{
	mEditorCore->requestDownloadVersion();
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