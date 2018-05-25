#include "txEngineRoot.h"
#include "txProperty.h"

#include "Layout.h"
#include "WindowFactory.h"
#include "WindowPrefabManager.h"
#include "WindowPrefab.h"
#include "WindowProperty.h"
#include "LayoutManager.h"
#include "WindowHeader.h"

#include "LayoutEditorCore.h"
#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "PrefabResource.h"

enum
{
	ID_REFRESH,
	ID_PREFAB_RESOURCE_LISTCTRL,
	ID_MENU_OPEN_FILE,
	ID_MENU_OPEN_DIR,
	ID_MENU_EDIT_PREFAB,
};

BEGIN_EVENT_TABLE(PrefabResource, EditorPanel)
EVT_TOOL(ID_REFRESH, PrefabResource::OnRefresh)
EVT_LISTBOX_DCLICK(ID_PREFAB_RESOURCE_LISTCTRL, PrefabResource::OnDoubleClick)
EVT_MENU(ID_MENU_OPEN_FILE, PrefabResource::OnMenuOpenFile)
EVT_MENU(ID_MENU_OPEN_DIR, PrefabResource::OnMenuOpenDir)
EVT_MENU(ID_MENU_EDIT_PREFAB, PrefabResource::OnMenuEditPrefab)
END_EVENT_TABLE()

PrefabResource::PrefabResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL)
{
	Setup();
}

PrefabResource::~PrefabResource()
{
	mResourceList->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(PrefabResource::OnRightUp), NULL, this);
	mResourceList->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(PrefabResource::OnRightDown), NULL, this);
}

void PrefabResource::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListBox(this, ID_PREFAB_RESOURCE_LISTCTRL, wxDefaultPosition, wxDefaultSize, 0, NULL, wxHSCROLL | wxVSCROLL);
	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);

	mResourceList->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(PrefabResource::OnRightDown), NULL, this);
	mResourceList->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(PrefabResource::OnRightUp), NULL, this);
	this->SetSizer(bSizer1);
	this->Layout();
}

void PrefabResource::refreshList()
{
	mResourceList->Clear();
	auto& prefabList = mWindowPrefabManager->getPrefabList();
	auto itr = prefabList.begin();
	auto itrEnd = prefabList.end();
	FOR_STL(prefabList, ; itr != itrEnd; ++itr)
	{
		mResourceList->Append(wxString(itr->first.c_str()));
	}
	END_FOR_STL(prefabList);
}

void PrefabResource::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void PrefabResource::OnDoubleClick(wxCommandEvent& event)
{
	txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	std::string itemName = event.GetString().ToStdString();
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout == NULL)
	{
		return;
	}
	std::string prefabPropertyName = txProperty::getPropertyName<WindowPropertyPrefab>();
	int selectionCount = selections.size();
	FOR_STL(selections, int i = 0; i < selectionCount; ++i)
	{
		CommandWindowSetProperty* cmdProperty = NEW_CMD(cmdProperty);
		cmdProperty->mPropertyName = prefabPropertyName;
		cmdProperty->mPropertyValue = itemName;
		cmdProperty->mOperator = this;
		mCommandSystem->pushCommand(cmdProperty, selections[i]);
	}
	END_FOR_STL(selections);
}

void PrefabResource::OnRightUp(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	ShowMenu(clientpt);
	event.Skip();
}

void PrefabResource::OnRightDown(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	int index = mResourceList->HitTest(clientpt);
	mResourceList->SetSelection(index);
	event.Skip();
}

void PrefabResource::OnMenuOpenFile(wxCommandEvent& event)
{
	WindowPrefab* curSelectPrefab = mWindowPrefabManager->getPrefab(mResourceList->GetStringSelection().ToStdString());
	EditorUtility::launchDefaultApplication(P_UI_WINDOW_PREFAB_PATH + curSelectPrefab->getFileName());
}

void PrefabResource::OnMenuOpenDir(wxCommandEvent& event)
{
	EditorUtility::launchDefaultBrowser(P_UI_WINDOW_PREFAB_PATH);
}

void PrefabResource::OnMenuEditPrefab(wxCommandEvent& event)
{
	mLayoutEditorCore->editPrefab(mResourceList->GetStringSelection().ToStdString());
}

void PrefabResource::ShowMenu(const wxPoint& pt)
{
#if wxUSE_MENUS  
	wxMenu menu;
	menu.Append(ID_MENU_OPEN_FILE, wxT("打开文件"));
	menu.Append(ID_MENU_OPEN_DIR, wxT("打开所在文件夹"));
	menu.Append(ID_MENU_EDIT_PREFAB, wxT("编辑预设"));
	PopupMenu(&menu, pt);
#endif
}