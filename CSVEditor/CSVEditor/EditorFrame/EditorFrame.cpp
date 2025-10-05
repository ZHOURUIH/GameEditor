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
	// �����༭����
	CreateEditorCore();

	mAuiManager.SetManagedWindow(this);
	// �����˵���
	CreateMenu();
	// ����������
	CreateToolBar();
	// ���������Ӵ���
	CreateWindows();
	// �����ײ���״̬��ʾ��
	CreateStatuBar();

	// �������ˢ��һ��ȫ���ؼ���ѡ��״̬
	RefreshAllMenuToolCheckState();
}

void EditorFrame::CreateMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// �ļ��˵�
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, "��\tCtrl+O", "Open a file");
	fileMenu->Append(wxID_NEW, "�½�\tCtrl+N", "New a file");
	fileMenu->Append(wxID_SAVE, "����\tCtrl+S", "Save a file");
	fileMenu->Append(wxID_EXIT, "�ر�\tAlt+F4", "Quit the program");
	menuBar->Append(fileMenu, "�ļ�");

	Bind(wxEVT_MENU, &EditorFrame::OnOpenFile, this, wxID_OPEN);
	Bind(wxEVT_MENU, &EditorFrame::OnNewFile, this, wxID_NEW);
	Bind(wxEVT_MENU, &EditorFrame::OnSaveFile, this, wxID_SAVE);
	Bind(wxEVT_MENU, &EditorFrame::OnExit, this, wxID_EXIT);

	// �༭�˵�
	wxMenu* editMenu = new wxMenu;
	editMenu->Append(wxID_COPY, "����\tCtrl+C", "copy");
	editMenu->Append(wxID_PASTE, "ճ��\tCtrl+V", "paste");
	editMenu->Append(ID_UNDO, "����\tCtrl+Z", "undo");
	editMenu->Append(ID_REDO, "����\tCtrl+Y", "redo");
	menuBar->Append(editMenu, "�༭");

	Bind(wxEVT_MENU, &EditorFrame::OnCopy, this, wxID_COPY);
	Bind(wxEVT_MENU, &EditorFrame::OnPaste, this, wxID_PASTE);
	Bind(wxEVT_MENU, &EditorFrame::OnUndo, this, ID_UNDO);
	Bind(wxEVT_MENU, &EditorFrame::OnRedo, this, ID_REDO);

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
	wxToolBar* toolbar = wxFrame::CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT);
	toolbar->AddTool(ID_UNDO, "����", wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR), wxArtProvider::GetBitmap(wxART_UNDO, wxART_TOOLBAR).ConvertToDisabled(), wxITEM_NORMAL, "�����ϴβ���", "�����ϴβ���");
	toolbar->AddTool(ID_REDO, "����", wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR), wxArtProvider::GetBitmap(wxART_REDO, wxART_TOOLBAR).ConvertToDisabled(), wxITEM_NORMAL, "�ָ��ϴγ����Ĳ���", "�ָ��ϴγ����Ĳ���");
	toolbar->AddTool(ID_ADD_ROW_TO_FIRST, "���뵽��һ��", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR), "���뵽��һ��");
	toolbar->AddTool(ID_ADD_ROW_TO_END, "���뵽���һ��", wxArtProvider::GetBitmap(wxART_PLUS, wxART_TOOLBAR), "���뵽���һ��");
	toolbar->Realize();
	mAuiManager.Update();
}

void EditorFrame::CreateWindows()
{
	mMainListWindow = new MainListWindow(this, wxMINIMIZE_BOX | wxMAXIMIZE_BOX);
	mMainListWindow->init();
	mAuiManager.AddPane(mMainListWindow, wxAuiPaneInfo().Name("MainListWindow").BestSize(wxSize(-1, -1)).FloatingSize(400, 600).Caption("�б�").Center().Dockable(true));
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
		const int ret = dialogYesNoCancel("�ļ�δ����,�Ƿ񱣴����˳�?", "���沢�˳�", "���������˳�", "ȡ���˳�");
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
	// �����رմ��ڵ��¼�
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
		const int ret = dialogYesNoCancel("�ļ�δ����,�Ƿ񱣴����˳�?", "���沢�˳�", "���������˳�", "ȡ���˳�");
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
	// �����Լ�������
	destroy();
	// ���ٴ���
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
	// ��ȡ�������ؼ�
	wxToolBar* toolbar = mEditorFrame->GetToolBar();
	if (toolbar != nullptr)
	{
		toolbar->EnableTool(ID_UNDO, mUndoManager->canUndo());
		toolbar->EnableTool(ID_REDO, mUndoManager->canRedo());
	}
	// ��ȡ�˵����ؼ�
	wxMenuBar* menuBar = mEditorFrame->GetMenuBar();
	if (menuBar != nullptr)
	{
		menuBar->Enable(ID_UNDO, mUndoManager->canUndo());
		menuBar->Enable(ID_REDO, mUndoManager->canRedo());
	}
}