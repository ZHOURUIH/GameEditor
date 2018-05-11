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

EditorFrame::EditorFrame(EditorApp* app, wxString title, wxPoint position, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, position, size),
CommandReceiver("EditorFrame"),
mApp(app),
mTimer(NULL),
mFileMenu(NULL),
mEditorCore(NULL),
mDownloadPanel(NULL),
mRunning(true),
mUpdateDone(false),
mAutoStartGame(true),
mAutoClose(false)
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
	mWindowIDList.insert(panel, toolID);																														\
	mWindowList.insert(panel->GetName().ToStdString(), panel);

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
	auto iterWindow = mWindowIDList.begin();
	auto iterWindowEnd = mWindowIDList.end();
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
	if (mEditorCore != NULL)
	{
		mEditorCore->update(elapsedTime);
		mDownloadPanel->update(elapsedTime);
	}
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
	EditorBusyInfo* busyInfo = mApp->getBusyInfo();
	if (type == CET_ERROR_LOG)
	{
		EditorUtility::logError(params[0]);
	}
	else if (type == CET_INFO_LOG)
	{
		EditorUtility::logInfo(params[0]);
	}
	else if (type == CET_STOP_PROGRAM)
	{
		mRunning = false;
	}
	else if (type == CET_START_DOWNLOAD_VERSION)
	{
		busyInfo->show();
		busyInfo->setBusyInfo(std::string("正在下载版本号...").c_str());
	}
	else if (type == CET_START_DOWNLOAD_LIST_FILE)
	{
		busyInfo->show();
		busyInfo->setBusyInfo(std::string("正在下载列表文件...").c_str());
	}
	// 版本文件下载完毕
	else if (type == CET_NEW_VERSION)
	{
		if (StringUtility::stringToBool(params[0]))
		{
			busyInfo->hide();
			Show(true);
			int ret = EditorUtility::askYesNODialog("发现有新版本,是否要更新?(更新会将所有配置文件恢复默认值)");
			mEditorCore->requestUpdateGame(ret == wxYES);
		}
		else
		{
			mDownloadPanel->finishDownload();
			busyInfo->hide();
			downloadDone(true);
		}
	}
	// 版本文件和列表文件检查完毕,开始下载资源
	else if (type == CET_START_DOWNLOAD)
	{
		mDownloadPanel->startDownload();
	}
	// 所有文件下载完毕,正在更新文件
	else if (type == CET_UPDATING_FILE)
	{
		busyInfo->show();
		busyInfo->setBusyInfo(std::string("正在更新文件...0%").c_str());
	}
	else if (type == CET_UPDATING_PROGRESS)
	{
		float progress = StringUtility::stringToFloat(params[0]);
		busyInfo->setBusyInfo((std::string("正在更新文件...") + StringUtility::floatToString(progress * 100.0f, 3) + "%").c_str());
	}
	// 更新完成,所有资源与服务器上同步完成
	else if (type == CET_UPDATE_DONE)
	{
		mDownloadPanel->finishDownload();
		downloadDone(true);
	}
	// 列表文件下载完毕,开始对比本地文件
	else if (type == CET_START_GENERATE_LOCAL_FILE)
	{
		mDownloadPanel->notifyStartGenerateLocalFile();
		busyInfo->show();
		busyInfo->setBusyInfo((std::string("正在检查本地文件...0/") + params[0]).c_str());
	}
	// 正在对比本地文件
	else if (type == CET_GENERATING_LOCAL_FILE)
	{
		busyInfo->setBusyInfo((std::string("正在检查本地文件...") + params[1] + "/" + params[0]).c_str());
	}
	// 文件对比完成
	else if (type == CET_FINISH_GENERATE_LOCAL_FILE)
	{
		mDownloadPanel->notifyFinishGenerateLocalFile();
		busyInfo->hide();
	}
	// 取消下载更新
	else if (type == CET_CANCEL_UPDATE)
	{
		downloadDone(false);
	}
	// 对比所有文件后未发现需要更新的文件
	else if (type == CET_NOTHING_UPDATE)
	{
		mDownloadPanel->notifyNothingUpdate();
		downloadDone(true);
	}
	// 有文件上传完毕
	else if (type == CET_FINISH_UPLOAD)
	{
		;
	}
	// 所有文件上传完毕
	else if (type == CET_ALL_UPLOADED)
	{
		uploadDone();
	}
}

void EditorFrame::uploadDone()
{
	// 上传完毕后如果自动关闭,则关闭程序
	if (mAutoClose)
	{
		mRunning = false;
	}
	// 不自动关闭,则开始下载版本文件判断是否需要更新
	else
	{
		mEditorCore->requestDownloadVersion();
	}
}

void EditorFrame::downloadDone(bool launch)
{
	mRunning = false;
	if (launch)
	{
		mUpdateDone = true;
		if (mAutoStartGame)
		{
			// 显示提示
			EditorBusyInfo* busyInfo = mApp->getBusyInfo();
			busyInfo->show();
			busyInfo->setBusyInfo("正在启动游戏...");
			// 启动游戏
			txUtility::launchExecutable(StringUtility::getParentDir(GAME_NAME), GAME_NAME);
		}
	}
}

wxWindow* EditorFrame::getWindow(const std::string& name)
{
	auto iter = mWindowList.find(name);
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

void EditorFrame::start(const txVector<std::pair<std::string, std::string>>& uploadFileList)
{
	if (uploadFileList.size() > 0)
	{
		// 请求开始上传
		mEditorCore->requestUploadFile(uploadFileList);
	}
	else
	{
		uploadDone();
	}
}

void EditorFrame::OnPanelShow(wxCommandEvent& event)
{
	int eventID = event.GetId();
	auto iterPanel = mWindowIDList.begin();
	auto iterPanelEnd = mWindowIDList.end();
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