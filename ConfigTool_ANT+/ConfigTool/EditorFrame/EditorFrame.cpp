#include "ConfigToolCore.h"
#include "CTEventSystem.h"
#include "WirelessDataManager.h"
#include "txMemoryTrace.h"
#include "ToolCoreUtility.h"
#include "ToolCoreLog.h"
#include "SpeedDataManager.h"

#include "EditorFrame.h"
#include "EditorApp.h"
#include "WirelessDevicePanel.h"
#include "SetupDevicePanel.h"
#include "EditorCommandHeader.h"
#include "SpeedDataPanel.h"
#include "HeartRatePanel.h"

#define NEW_PANEL(type, panel, title, toolID, size, floatingSize, dock)																										\
	panel = new type(this, wxDefaultPosition, size, wxMINIMIZE | wxMAXIMIZE, wxT(TOSTRING(type)));																			\
	mAuiManager.AddPane(panel, wxAuiPaneInfo().Name(panel->GetName()).BestSize(wxSize(-1, -1)).FloatingSize(floatingSize).Caption(wxT(title)).##dock##().Dockable(true));	\
	mWindowIDList.insert(panel, toolID);																																	\
	mWindowList.insert(panel->GetName().ToStdString(), panel);																												\
	panel->init();

#define NEW_PANEL_LEFT(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(400, -1), wxSize(300, 800), Left)
#define NEW_PANEL_RIGHT(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(400, -1), wxSize(300, 800), Right)
#define NEW_PANEL_BOTTOM(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(-1, 400), wxSize(1200, 400), Bottom)
#define NEW_PANEL_CENTER(type, panel, title, toolID) NEW_PANEL(type, panel, title, toolID, wxSize(-1, -1), wxSize(-1, -1), Center)
#define NEW_PANEL_LEFT_FAST(className, toolID) NEW_PANEL_LEFT(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_RIGHT_FAST(className, toolID) NEW_PANEL_RIGHT(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_BUTTOM_FAST(className, toolID) NEW_PANEL_BOTTOM(className, m##className, TOSTRING(className), toolID)
#define NEW_PANEL_CENTER_FAST(className, toolID) NEW_PANEL_CENTER(className, m##className, TOSTRING(className), toolID)

enum
{
	ID_TIMER,
	ID_MENU_EXIT,
	ID_TOOL_WIRELESS_DEVICE_PANEL,
	ID_TOOL_SETUP_DEVICE_PANEL,
	ID_TOOL_SPEED_DATA_PANEL,
	ID_TOOL_HEART_RATE_PANEL,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_EXIT, OnExit)
EVT_TOOL(ID_TOOL_WIRELESS_DEVICE_PANEL, OnWirelessDevicePanel)
EVT_TOOL(ID_TOOL_SETUP_DEVICE_PANEL, OnSetupDevicePanel)
EVT_TOOL(ID_TOOL_SPEED_DATA_PANEL, OnSpeedDataPanel)
EVT_TOOL(ID_TOOL_HEART_RATE_PANEL, OnHeartRatePanel)

END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxSize& size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
txCommandReceiver(TOSTRING(EditorFrame)),
mTimer(NULL),
mFileMenu(NULL),
mWirelessDevicePanel(NULL),
mSetupDevicePanel(NULL),
mConfigToolCore(NULL),
mSpeedDataPanel(NULL),
mHeartRatePanel(NULL)
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
	mTimer = TRACE_NEW(wxTimer, mTimer);
	mTimer->Start(1);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	TRACE_DELETE(mTimer);
	unregisterEditorCoreEventHandler();
	TRACE_DELETE(mConfigToolCore);
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
	mWindowToolBar->AddTool(ID_TOOL_WIRELESS_DEVICE_PANEL, wxT("无线设备窗口"), wxBITMAP(IDB_WIRELESS_DEVICE_ICON), wxT("无线设备数据窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_SETUP_DEVICE_PANEL, wxT("设备配置窗口"), wxBITMAP(IDB_DEVICE_LIST_ICON), wxT("无线设备配置窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_SPEED_DATA_PANEL, wxT("有线设备速度数据窗口"), wxBITMAP(IDB_SPEED_ICON), wxT("有线设备速度数据窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_HEART_RATE_PANEL, wxT("有线设备心率数据窗口"), wxBITMAP(IDB_HEART_RATE_ICON), wxT("有线设备心率数据窗口"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("窗口工具栏")).ToolbarPane().Top());
	
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	NEW_PANEL_CENTER_FAST(WirelessDevicePanel, ID_TOOL_WIRELESS_DEVICE_PANEL);
	NEW_PANEL_CENTER_FAST(SetupDevicePanel, ID_TOOL_SETUP_DEVICE_PANEL);
	NEW_PANEL_CENTER_FAST(SpeedDataPanel, ID_TOOL_SPEED_DATA_PANEL);
	NEW_PANEL_CENTER_FAST(HeartRatePanel, ID_TOOL_HEART_RATE_PANEL);
	mAuiManager.Update();
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
	mConfigToolCore = TRACE_NEW(ConfigToolCore, mConfigToolCore);
	mConfigToolCore->init();
	registerEditorCoreEventHandler();
	mConfigToolCore->notifyInitDone();
}

void EditorFrame::RefreshAllResource()
{
	mSpeedDataPanel->refresh();
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	auto iterWindow = mWindowIDList.begin();
	auto iterWindowEnd = mWindowIDList.end();
	for (int i = 0; iterWindow != iterWindowEnd; (++iterWindow, ++i))
	{
		bool check = i == 0;
		mAuiManager.GetPane(iterWindow->first).Show(check);
		mWindowToolBar->ToggleTool(iterWindow->second, check);
	}
	mAuiManager.Update();
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
	if (mConfigToolCore == NULL)
	{
		return;
	}
	mConfigToolCore->update(elapsedTime);
	mWirelessDevicePanel->update(elapsedTime);
	mSetupDevicePanel->update(elapsedTime);
	mSpeedDataPanel->update(elapsedTime);
	mHeartRatePanel->update(elapsedTime);
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
	return wxFrame::MSWWindowProc(message, wParam, lParam);
}

void EditorFrame::registerEditorCoreEventHandler()
{
	mConfigToolCore->getEventSystem()->registeAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEventHandler()
{
	mConfigToolCore->getEventSystem()->unregisteAllEvent(this);
}

void EditorFrame::onEditorCoreEvent(const CORE_EVENT& type, txVector<std::string>& params)
{
	if (type == CE_ERROR_LOG)
	{
		EditorUtility::logError(params[0]);
	}
	else if (type == CE_INFO_LOG)
	{
		EditorUtility::logInfo(params[0]);
	}
	else if (type == CE_HEART_RATE_TIME)
	{
		if (mHeartRatePanel != NULL)
		{
			mHeartRatePanel->refreshTime();
		}
	}
	else if (type == CE_REGISTE_DEVICE_LIST_MODIFIED)
	{
		if (mSetupDevicePanel != NULL)
		{
			mSetupDevicePanel->notifyRegisteListModified(StringUtility::stringToBool(params[0]));
		}
	}
	else if (type == CE_UPLOAD_STATE)
	{
		if (mSetupDevicePanel)
		{
			mSetupDevicePanel->notifyUploadState((UPLOAD_STATE)StringUtility::stringToInt(params[0]));
		}
	}
	else if (type == CE_US_UNUPLOAD)
	{
		if (mSetupDevicePanel)
		{
			mSetupDevicePanel->notifyUploadState((UPLOAD_STATE)StringUtility::stringToInt(params[0]));
		}
	}
	else if (type == CE_UPLOAD_FAILED)
	{
		if (mSetupDevicePanel)
		{
			mSetupDevicePanel->notifyUploadState((UPLOAD_STATE)StringUtility::stringToInt(params[0]));
		}
	}

}

EditorPanel* EditorFrame::getPanel(const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		return iter->second;
	}
	return NULL;
}

int EditorFrame::getPanelID(const std::string& name)
{
	EditorPanel* window = getPanel(name);
	auto iterWindow = mWindowIDList.find(window);
	if (iterWindow == mWindowIDList.end())
	{
		return -1;
	}
	return iterWindow->second;
}

void EditorFrame::showPanel(wxWindow* panel, bool show, bool update)
{
	if (mAuiManager.GetPane(panel).IsShown() != show)
	{
		mAuiManager.GetPane(panel).Show(show);
		if (update)
		{
			mAuiManager.Update();
		}
	}
}

void EditorFrame::hideAllPanel()
{
	auto iter = mWindowList.begin();
	auto iterEnd = mWindowList.end();
	for (; iter != iterEnd; ++iter)
	{
		showPanel(iter->second, false, false);
	}
	mAuiManager.Update();
}

void EditorFrame::OnWirelessDevicePanel(wxCommandEvent& event)
{
	hideAllPanel();
	showPanel(mWirelessDevicePanel, event.IsChecked());
}

void EditorFrame::OnSetupDevicePanel(wxCommandEvent& event)
{
	hideAllPanel();
	showPanel(mSetupDevicePanel, event.IsChecked());
}

void EditorFrame::OnSpeedDataPanel(wxCommandEvent& event)
{
	hideAllPanel();
	showPanel(mSpeedDataPanel, event.IsChecked());
}

void EditorFrame::OnHeartRatePanel(wxCommandEvent& event)
{
	hideAllPanel();
	showPanel(mHeartRatePanel, event.IsChecked());
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