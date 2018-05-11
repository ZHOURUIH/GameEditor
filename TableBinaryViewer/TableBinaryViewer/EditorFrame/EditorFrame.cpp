#include "EditorFrame.h"
#include "EditorApp.h"
#include "MainViewerWindow.h"
#include "TableBinaryResourceWindow.h"
#include "TableTxtResourceWindow.h"
#include "SupportedTableWindow.h"
#include "DataBase.h"

enum
{
	ID_TIMER,
	ID_MENU_SAVE,
	ID_TOOL_SAVE,
	ID_EDIT_TABLE
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_SAVE, OnSave)
EVT_TOOL(ID_TOOL_SAVE, OnSave)
EVT_TOOL(ID_EDIT_TABLE, OnEditTable)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
mTimer(NULL),
mDataBase(NULL),
mMainViewerWindow(NULL),
mTableBinaryResourceWindow(NULL),
mTableTxtResourceWindow(NULL),
mSupportedTableWindow(NULL),
mAppNormalName(title),
mDataModified(false)
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
	if (isModified())
	{
		int ret = wxMessageBox(wxT("是否保存当前表格?"), "提示", wxCANCEL | wxYES | wxNO);
		if (ret == wxYES)
		{
			saveData();
		}
		else if (ret == wxNO)
		{
			;
		}
		else if (ret == wxCANCEL)
		{
			return;
		}
	}
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
	mFileMenu->Append(ID_MENU_SAVE, wxT("保存表格\tCtrl-S"));
	menuBar->Append(mFileMenu, "文件");

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	mFileToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mFileToolBar->SetToolBitmapSize(wxSize(16, 16));
	mFileToolBar->AddTool(ID_TOOL_SAVE, wxT("保存表格"), wxBITMAP(IDB_SAVE), wxT("保存表格"));
	mFileToolBar->AddTool(ID_EDIT_TABLE, wxT("设计表格"), wxBITMAP(IDB_EDITTABLE), wxT("设计表格"));
	mFileToolBar->Realize();
	mAuiManager.AddPane(mFileToolBar, wxAuiPaneInfo().Name(wxT("FileToolBar")).Caption(wxT("文件工具栏")).ToolbarPane().Top());
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainViewerWindow = new MainViewerWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mAuiManager.AddPane(mMainViewerWindow, wxAuiPaneInfo().Name("MainViewerWindow").BestSize(wxSize(500, -1)).FloatingSize(400, 600).Caption("数据内容").Center());
	mMainViewerWindow->Show();

	mTableBinaryResourceWindow = new TableBinaryResourceWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("TableBinaryResourceWindow"));
	mAuiManager.AddPane(mTableBinaryResourceWindow, wxAuiPaneInfo().Name("TableBinaryResourceWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("二进制数据列表").Right());
	mTableBinaryResourceWindow->Show();

	mTableTxtResourceWindow = new TableTxtResourceWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("TableTxtResourceWindow"));
	mAuiManager.AddPane(mTableTxtResourceWindow, wxAuiPaneInfo().Name("TableTxtResourceWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("文本数据列表").Right());
	mTableTxtResourceWindow->Show();

	mSupportedTableWindow = new SupportedTableWindow(this, wxPoint(-1, -1), wxSize(400, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT("SupportedTableWindow"));
	mAuiManager.AddPane(mSupportedTableWindow, wxAuiPaneInfo().Name("SupportedTableWindow").BestSize(wxSize(300, -1)).FloatingSize(400, 600).Caption("支持的表格格式").Left());
	mSupportedTableWindow->Show();

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
	mDataBase = TRACE_NEW(DataBase, mDataBase);
	mDataBase->setErrorCallback(EditorFrame::errorMessage);
	mDataBase->init(false);
}

void EditorFrame::RefreshAllResource()
{
	mTableBinaryResourceWindow->refresh();
	mTableTxtResourceWindow->refresh();
	mSupportedTableWindow->refresh();
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

void EditorFrame::OnSave(wxCommandEvent& event)
{
	saveData();
}

void EditorFrame::OnEditTable(wxCommandEvent& event)
{
	if (mTableName != "")
	{
		mSupportedTableWindow->editTable(const_cast<DataTemplate*>(mDataBase->getDataTemplate(mTableName)));
	}
}

void EditorFrame::notifyOpenTable(std::string tableName)
{
	mTableName = tableName;
	refreshTitle();
}

void EditorFrame::setDataModified(bool modified)
{
	mDataModified = modified;
	refreshTitle();
}

void EditorFrame::saveData()
{
	mDataBase->writeTemplateConfig(mTableName);
	mDataBase->writeBinaryFile(mTableName);
	setDataModified(false);
}

void EditorFrame::refreshTitle()
{
	if (mDataModified)
	{
		SetTitle(mAppNormalName + " - " + mTableName + " *");
	}
	else
	{
		SetTitle(mAppNormalName + " - " + mTableName);
	}
}