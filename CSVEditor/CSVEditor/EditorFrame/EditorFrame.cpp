#include "AllHeader.h"

enum
{
	ID_TIMER,
	ID_UNDO,
	ID_REDO,
	ID_ADD_ROW_TO_FIRST,
	ID_ADD_ROW_TO_END,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_TOOL(ID_UNDO, EditorFrame::OnUndo)
EVT_TOOL(ID_REDO, EditorFrame::OnRedo)
EVT_TOOL(ID_ADD_ROW_TO_FIRST, EditorFrame::OnAddRowToFirst)
EVT_TOOL(ID_ADD_ROW_TO_END, EditorFrame::OnAddRowToEnd)
EVT_CLOSE(OnCloseWindow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size):
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
	mEditorFrame = this;
	mCSVEditor->setDirtyCallback(onDirty);
	mUndoManager->setUndoChangeCallback(onUndoChanged);
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
	if (mTimer != nullptr)
	{
		delete mTimer;
		mTimer = nullptr;
	}
	if (mMainListWindow != nullptr)
	{
		delete mMainListWindow;
		mMainListWindow = nullptr;
	}
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
}

void EditorFrame::CreateMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// 文件菜单
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, "打开\tCtrl+O", "Open a file");
	fileMenu->Append(wxID_NEW, "新建\tCtrl+N", "New a file");
	fileMenu->Append(wxID_SAVE, "保存\tCtrl+S", "Save a file");
	fileMenu->Append(wxID_EXIT, "关闭\tAlt+F4", "Quit the program");
	menuBar->Append(fileMenu, "文件");

	Bind(wxEVT_MENU, &EditorFrame::OnOpenFile, this, wxID_OPEN);
	Bind(wxEVT_MENU, &EditorFrame::OnNewFile, this, wxID_NEW);
	Bind(wxEVT_MENU, &EditorFrame::OnSaveFile, this, wxID_SAVE);
	Bind(wxEVT_MENU, &EditorFrame::OnExit, this, wxID_EXIT);

	// 编辑菜单
	wxMenu* editMenu = new wxMenu;
	editMenu->Append(wxID_COPY, "复制\tCtrl+C", "copy");
	editMenu->Append(wxID_PASTE, "粘贴\tCtrl+V", "paste");
	editMenu->Append(ID_UNDO, "撤销\tCtrl+Z", "undo");
	editMenu->Append(ID_REDO, "重做\tCtrl+Y", "redo");
	menuBar->Append(editMenu, "编辑");

	Bind(wxEVT_MENU, &EditorFrame::OnCopy, this, wxID_COPY);
	Bind(wxEVT_MENU, &EditorFrame::OnPaste, this, wxID_PASTE);
	Bind(wxEVT_MENU, &EditorFrame::OnUndo, this, ID_UNDO);
	Bind(wxEVT_MENU, &EditorFrame::OnRedo, this, ID_REDO);

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	wxToolBar* toolbar = wxFrame::CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT);
	toolbar->AddTool(ID_UNDO, "撤销", wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR), wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR).ConvertToDisabled(), wxITEM_NORMAL, "撤销上次操作", "撤销上次操作");
	toolbar->AddTool(ID_REDO, "重做", wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR), wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR).ConvertToDisabled(), wxITEM_NORMAL, "恢复上次撤销的操作", "恢复上次撤销的操作");
	toolbar->AddTool(ID_ADD_ROW_TO_FIRST, "插入到第一行", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR), "插入到第一行");
	toolbar->AddTool(ID_ADD_ROW_TO_END, "插入到最后一行", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR), "插入到最后一行");
	toolbar->Realize();
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainListWindow = new MainListWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mMainListWindow->init();
	mAuiManager.AddPane(mMainListWindow, wxAuiPaneInfo().Name("MainListWindow").BestSize(wxSize(-1, -1)).FloatingSize(400, 600).Caption("列表").Center().Dockable(true));
	mMainListWindow->Show();

	mAuiManager.Update();
}

void EditorFrame::CreateStatuBar()
{
	CreateStatusBar(2);
}

void EditorFrame::RefreshAllMenuToolCheckState()
{
	onUndoChanged();
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

void EditorFrame::OnExit(wxCommandEvent& event)
{
	if (mCSVEditor->isOpened() && mCSVEditor->isDirty())
	{
		const int ret = dialogYesNoCancel("文件未保存,是否保存再退出?", "保存并退出", "不保存且退出", "取消退出");
		if (ret == wxID_YES)
		{
			mCSVEditor->save();
		}
		else if (ret == wxID_NO) {}
		else if (ret == wxID_CANCEL)
		{
			return;
		}
	}
	// 发出关闭窗口的事件
	Close(true);
}

void EditorFrame::OnOpenFile(wxCommandEvent& event)
{
	mMainListWindow->openFile();
}

void EditorFrame::OnNewFile(wxCommandEvent& event)
{
	mMainListWindow->newFile();
}

void EditorFrame::OnSaveFile(wxCommandEvent& event)
{
	mMainListWindow->save();
}

void EditorFrame::OnCopy(wxCommandEvent& event)
{
	mMainListWindow->copySelection();
}

void EditorFrame::OnPaste(wxCommandEvent& event)
{
	mMainListWindow->pasteSelection();
}

void EditorFrame::OnUndo(wxCommandEvent& event)
{
	if (!mUndoManager->canUndo())
	{
		return;
	}
	mUndoManager->undo();
}

void EditorFrame::OnRedo(wxCommandEvent& event)
{
	if (!mUndoManager->canRedo())
	{
		return;
	}
	mUndoManager->redo();
}

void EditorFrame::OnAddRowToFirst(wxCommandEvent& event)
{
	mMainListWindow->addRowToFirst({});
}

void EditorFrame::OnAddRowToEnd(wxCommandEvent& event)
{
	mMainListWindow->addRowToEnd({});
}

void EditorFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	if (mCSVEditor->isOpened() && mCSVEditor->isDirty())
	{
		const int ret = dialogYesNoCancel("文件未保存,是否保存再退出?", "保存并退出", "不保存且退出", "取消退出");
		if (ret == wxID_YES)
		{
			mCSVEditor->save();
		}
		else if (ret == wxID_NO) {}
		else if (ret == wxID_CANCEL)
		{
			return;
		}
	}
	// 销毁自己的数据
	destroy();
	// 销毁窗口
	Destroy();
}

void EditorFrame::onDirty()
{
	string title = "CSVEditor";
	if (mCSVEditor->isOpened())
	{
		title += " " + mCSVEditor->getFilePath();
		title += " " + mCSVEditor->getTableName();
	}
	if (mCSVEditor->isDirty())
	{
		title += " *";
	}
	mEditorFrame->SetTitle(title);
}

void EditorFrame::onUndoChanged()
{
	// 获取工具栏控件
	wxToolBar* toolbar = mEditorFrame->GetToolBar();
	if (toolbar != nullptr)
	{
		toolbar->EnableTool(ID_UNDO, mUndoManager->canUndo());
		toolbar->EnableTool(ID_REDO, mUndoManager->canRedo());
	}
	// 获取菜单栏控件
	wxMenuBar* menuBar = mEditorFrame->GetMenuBar();
	if (menuBar != nullptr)
	{
		menuBar->Enable(ID_UNDO, mUndoManager->canUndo());
		menuBar->Enable(ID_REDO, mUndoManager->canRedo());
	}
}