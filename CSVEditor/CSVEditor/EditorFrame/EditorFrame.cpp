#include "AllHeader.h"

enum
{
	ID_TIMER,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)

END_EVENT_TABLE()

EditorFrame::EditorFrame(wxString title, wxSize size):
	wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size)
{
	mEditorFrame = this;
	mCSVEditor->setDirtyCallback(onDirty);
}

EditorFrame::~EditorFrame()
{}

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
	if (mMainListWindow != NULL)
	{
		delete mMainListWindow;
		mMainListWindow = NULL;
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

	// ˢ��ȫ����Դ�б�
	RefreshAllResource();
}

void EditorFrame::CreateMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// �ļ��˵�
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, "��\tCtrl+O", "Open a file");
	fileMenu->Append(wxID_SAVE, "����\tCtrl+S", "Save a file");
	fileMenu->Append(wxID_EXIT, "�ر�\tAlt+F4", "Quit the program");
	menuBar->Append(fileMenu, "�ļ�");

	Bind(wxEVT_MENU, &EditorFrame::OnOpenFile, this, wxID_OPEN);
	Bind(wxEVT_MENU, &EditorFrame::OnSaveFile, this, wxID_SAVE);
	Bind(wxEVT_MENU, &EditorFrame::OnExit, this, wxID_EXIT);

	// �༭�˵�
	wxMenu* editMenu = new wxMenu;
	editMenu->Append(wxID_COPY, "����\tCtrl+C", "copy");
	editMenu->Append(wxID_PASTE, "ճ��\tCtrl+V", "paste");
	menuBar->Append(editMenu, "�༭");

	Bind(wxEVT_MENU, &EditorFrame::OnCopy, this, wxID_COPY);
	Bind(wxEVT_MENU, &EditorFrame::OnPaste, this, wxID_PASTE);

	SetMenuBar(menuBar);
}

void EditorFrame::CreateToolBar()
{
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

void EditorFrame::UpdateStatus()
{
	;
}

void EditorFrame::CreateEditorCore()
{
	
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
	// �����رմ��ڵ��¼�
	Close(true);
}

void EditorFrame::OnOpenFile(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(this, _("Open CSV file"), "", "", "CSV files (*.csv)|*.csv|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	mCSVEditor->closeFile();
	mCSVEditor->openFile(openFileDialog.GetPath().ToStdString());
	if (!mCSVEditor->isOpened())
	{
		wxMessageBox("�ļ���ʧ��,�ļ����ܱ�ռ�û����ļ������޷�����", "����", wxOK | wxICON_ERROR);
		return;
	}
	mMainListWindow->initData(mCSVEditor);
}

void EditorFrame::OnSaveFile(wxCommandEvent& event)
{
	mCSVEditor->save();
}

void EditorFrame::OnCopy(wxCommandEvent& event)
{
	mMainListWindow->CopySelection();
}

void EditorFrame::OnPaste(wxCommandEvent& event)
{
	mMainListWindow->PasteSelection();
}

void EditorFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
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