#include "txEngineRoot.h"

#include "LayoutManager.h"
#include "Layout.h"

#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "LayoutResource.h"

enum
{
	ID_REFRESH,
	ID_LAYOUT_RESOURCE_LISTCTRL,
	ID_LISTBOX_RIGHT_OPEN_FILE,
	ID_LISTBOX_RIGHT_OPEN_DIR,
};

BEGIN_EVENT_TABLE(LayoutResource, EditorPanel)
EVT_TOOL(ID_REFRESH, LayoutResource::OnRefresh)
EVT_LISTBOX_DCLICK(ID_LAYOUT_RESOURCE_LISTCTRL, LayoutResource::OnDoubleClick)
EVT_MENU(ID_LISTBOX_RIGHT_OPEN_FILE, LayoutResource::OnOpenFile)
EVT_MENU(ID_LISTBOX_RIGHT_OPEN_DIR, LayoutResource::OnOpenDir)
END_EVENT_TABLE()

LayoutResource::LayoutResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL)
{
	Setup();
}

LayoutResource::~LayoutResource()
{
	mResourceList->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(LayoutResource::OnRightUp), NULL, this);
	mResourceList->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(LayoutResource::OnRightDown), NULL, this);
}

void LayoutResource::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListBox(this, ID_LAYOUT_RESOURCE_LISTCTRL, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxHSCROLL | wxVSCROLL);
	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);

	mResourceList->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(LayoutResource::OnRightDown), NULL, this);
	mResourceList->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(LayoutResource::OnRightUp), NULL, this);

	this->SetSizer(bSizer1);
	this->Layout();
}

void LayoutResource::refreshList()
{
	mResourceList->Clear();
	txVector<std::string> layoutFiles;
	txFileUtility::findFiles(P_UI_LAYOUT_PATH, layoutFiles, ".json");
	int fileCount = layoutFiles.size();
	FOR_STL(layoutFiles, int i = 0; i < fileCount; ++i)
	{
		std::string fileName = txStringUtility::getFileName(layoutFiles[i]);
		mResourceList->Append(wxString(fileName.c_str()));
	}
	END_FOR_STL(layoutFiles);
}

void LayoutResource::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void LayoutResource::OnDoubleClick(wxCommandEvent& event)
{
	std::string itemName = event.GetString().ToStdString();
	mEditorFrame->OpenFile(itemName);
}

void LayoutResource::OnRightUp(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	ShowMenu(clientpt);
	event.Skip();
}
void LayoutResource::OnRightDown(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	int index = mResourceList->HitTest(clientpt);
	mResourceList->SetSelection(index);
	event.Skip();
}

void LayoutResource::OnOpenFile(wxCommandEvent& event)
{
	EditorUtility::launchDefaultApplication(P_UI_LAYOUT_PATH + mResourceList->GetStringSelection().ToStdString());
}

void LayoutResource::OnOpenDir(wxCommandEvent& event)
{
	EditorUtility::launchDefaultBrowser(P_UI_LAYOUT_PATH);
}

void LayoutResource::ShowMenu(const wxPoint& pt)
{
#if wxUSE_MENUS  
	wxMenu menu;
	menu.Append(ID_LISTBOX_RIGHT_OPEN_FILE, wxT("打开文件"));
	menu.Append(ID_LISTBOX_RIGHT_OPEN_DIR, wxT("打开所在文件夹"));
	PopupMenu(&menu, pt);// 显示弹出菜单  
#endif
}