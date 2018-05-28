#include "txEngineRoot.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "WindowFactory.h"
#include "WindowTemplateManager.h"
#include "LayoutManager.h"

#include "LayoutEditorCore.h"
#include "EditorCommandHeader.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "WindowTemplate.h"

enum
{
	ID_REFRESH,
	ID_WINDOW_TEMPLATE_LISTCTRL,
	ID_LISTCTRL_RIGHT_OPEN_FILE,
	ID_LISTCTRL_RIGHT_OPEN_DIR,
};

BEGIN_EVENT_TABLE(WindowTemplate, EditorPanel)
EVT_TOOL(ID_REFRESH, WindowTemplate::OnRefresh)
EVT_LIST_ITEM_ACTIVATED(ID_WINDOW_TEMPLATE_LISTCTRL, WindowTemplate::OnDoubleClick)
EVT_MENU(ID_LISTCTRL_RIGHT_OPEN_FILE, WindowTemplate::OnOpenFile)
EVT_MENU(ID_LISTCTRL_RIGHT_OPEN_DIR, WindowTemplate::OnOpenDir)
EVT_CONTEXT_MENU(WindowTemplate::OnContexMenu)
END_EVENT_TABLE()

WindowTemplate::WindowTemplate(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name),
mToolBar(NULL)
{
	Setup();
}

void WindowTemplate::Setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mTemplateList = new wxListCtrl(this, ID_WINDOW_TEMPLATE_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	mTemplateList->InsertColumn(0, wxT(""), 0, 300);
	bSizer1->Add(mTemplateList, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void WindowTemplate::refreshList()
{
	mTemplateList->DeleteAllItems();
	auto& templateList = mWindowTemplateManager->getTemplateList();
	auto itr = templateList.begin();
	auto itrEnd = templateList.end();
	FOR_STL(templateList, ; itr != itrEnd; ++itr)
	{
		mTemplateList->InsertItem(mTemplateList->GetItemCount(), wxString(itr->first.c_str()));
	}
	END_FOR_STL(templateList);
}

void WindowTemplate::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void WindowTemplate::OnDoubleClick(wxListEvent& event)
{
	auto& selections = mLayoutEditorCore->getSelections();
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	if (layout == NULL)
	{
		return;
	}

	// 根据模板创建一个窗口,然后添加到选中窗口之下
	std::string itemName = mTemplateList->GetItemText(event.GetItem());
	Window* window = Layout::createIndependentWindow(mWindowTemplateManager->getTemplate(itemName), EMPTY_STRING, true);
	if (window == NULL)
	{
		return;
	}
	Window* windowParent = NULL;
	if (selections.size() > 0)
	{
		windowParent = selections[0];
	}
	mLayoutEditorCore->addWindow(layout, windowParent, window, -1);
}

void WindowTemplate::OnContexMenu(wxContextMenuEvent& event)
{
	wxPoint clientpt = mTemplateList->ScreenToClient(event.GetPosition());
	int flag;
	int index = mTemplateList->HitTest(clientpt, flag);
	mSelectText = mTemplateList->GetItemText(index);

	ShowMenu(clientpt);
	event.Skip();
}

void WindowTemplate::OnOpenFile(wxCommandEvent& event)
{
	Window* selTemplate = mWindowTemplateManager->getTemplate(mSelectText.ToStdString());
	EditorUtility::launchDefaultApplication(P_UI_WINDOW_TEMPLATE_PATH + mWindowTemplateManager->getTemplateFileName(selTemplate->getName()));
}

void WindowTemplate::OnOpenDir(wxCommandEvent& event)
{
	EditorUtility::launchDefaultBrowser(P_UI_WINDOW_TEMPLATE_PATH);
}

void WindowTemplate::ShowMenu(const wxPoint& pt)
{
#if wxUSE_MENUS  
	wxMenu menu;
	menu.Append(ID_LISTCTRL_RIGHT_OPEN_FILE, wxT("打开文件"));
	menu.Append(ID_LISTCTRL_RIGHT_OPEN_DIR, wxT("打开所在文件夹"));
	PopupMenu(&menu, pt);
#endif
}