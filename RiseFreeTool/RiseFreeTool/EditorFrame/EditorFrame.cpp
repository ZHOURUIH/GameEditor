#include "RiseFreeToolCore.h"
#include "EventSystem.h"
#include "txMemeryTrace.h"
#include "SpeedDataManager.h"
#include "ComManager.h"

#include "EditorFrame.h"
#include "EditorApp.h"
#include "SpeedDataPanel.h"
#include "EditorCommandHeader.h"

enum
{
	ID_TIMER,
	ID_MENU_EXIT,
	ID_TOOL_SPEED_DATA_PANEL,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_EXIT, OnExit)
EVT_TOOL(ID_TOOL_SPEED_DATA_PANEL, OnSpeedDataPanel)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
CommandReceiver("EditorFrame"),
mTimer(NULL),
mFileMenu(NULL),
mSpeedDataPanel(NULL),
mRiseFreeToolCore(NULL)
{
	;
}

EditorFrame::~EditorFrame()
{
	destroy();
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
	TRACE_DELETE(mRiseFreeToolCore);
	TRACE_DELETE(mTimer);
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
	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_SPEED_DATA_PANEL, wxT("速度数据窗口"), wxBITMAP(IDB_SPEED_ICON), wxT("显示速度数据窗口"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("窗口工具栏")).ToolbarPane().Top());
	
	mAuiManager.Update();
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
	NEW_PANEL_CENTER_FAST(SpeedDataPanel, ID_TOOL_SPEED_DATA_PANEL);
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
	mRiseFreeToolCore = TRACE_NEW(RiseFreeToolCore, mRiseFreeToolCore);
	mRiseFreeToolCore->init();
	registerEditorCoreEventHandler();
	mRiseFreeToolCore->notifyInitDone();
}

void EditorFrame::RefreshAllResource()
{
	mComManager->checkDevice();
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	txMap<wxWindow*, int>::iterator iterWindow = mWindowIDList.begin();
	txMap<wxWindow*, int>::iterator iterWindowEnd = mWindowIDList.end();
	for (; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowToolBar->ToggleTool(iterWindow->second, check);
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
	if (mRiseFreeToolCore == NULL)
	{
		return;
	}
	mRiseFreeToolCore->update(elapsedTime);
	mSpeedDataPanel->update();
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

WXLRESULT EditorFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	if (message == WM_DEVICECHANGE)
	{
		// 移除设备,添加设备,设备变化(实际参数值一直都是设备变化)
		if (wParam == DBT_DEVICEREMOVECOMPLETE || wParam == DBT_DEVICEARRIVAL || wParam == DBT_DEVNODES_CHANGED)
		{
			mComManager->checkDevice();
		}
	}
	return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void EditorFrame::registerEditorCoreEventHandler()
{
	mRiseFreeToolCore->getEventSystem()->registerAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEventHandler()
{
	mRiseFreeToolCore->getEventSystem()->unregisterAllEvent(this);
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
	else if (type == CET_DEVICE_LIST_CHANGED)
	{
		mSpeedDataPanel->notifyDeviceListChanged();
	}
	else if (type == CET_CUR_DEVICE_CHANGED)
	{
		mSpeedDataPanel->notifyCurDeviceChanged(params[0]);
	}
	else if (type == CET_DEVICE_OPENED)
	{
		mSpeedDataPanel->notifyDeviceOpen(true);
	}
	else if (type == CET_DEVICE_CLOSED)
	{
		mSpeedDataPanel->notifyDeviceOpen(false);
	}
}

wxWindow* EditorFrame::getWindow(std::string name)
{
	txMap<std::string, wxWindow*>::iterator iter = mWindowList.find(name);
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

void EditorFrame::OnSpeedDataPanel(wxCommandEvent& event)
{
	showPanel(mSpeedDataPanel, event.IsChecked());
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