#include "EditorApp.h"
#include "EditorFrame.h"
#include "SupportedTableWindow.h"
#include "DataDefine.h"
#include "DataBase.h"
#include "DataTemplate.h"
#include "TableBinaryResourceWindow.h"
#include "ConfigDataDialog.h"

enum
{
	ID_TOOL_REFRESH,
	ID_TOOL_CREATE_TABLE,
	ID_TABLE_RESOURCE_LISTBOX,
	ID_NEW_DATA,
};

BEGIN_EVENT_TABLE(SupportedTableWindow, wxPanel)

EVT_SHOW(SupportedTableWindow::OnShow)
EVT_MENU(ID_NEW_DATA, SupportedTableWindow::OnNewData)
EVT_TOOL(ID_TOOL_REFRESH, SupportedTableWindow::OnRefresh)
EVT_TOOL(ID_TOOL_CREATE_TABLE, OnCreateTable)

END_EVENT_TABLE()

SupportedTableWindow::SupportedTableWindow(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
wxPanel(parent, wxID_ANY, position, size, style, name)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

SupportedTableWindow::~SupportedTableWindow()
{
	mResourceList->Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(SupportedTableWindow::OnRightUp), NULL, this);
	mResourceList->Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(SupportedTableWindow::OnRightDown), NULL, this);
}

void SupportedTableWindow::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_TOOL_REFRESH, wxT("刷新资源"), wxBITMAP(IDB_REFRESH), wxT("刷新资源"));
	mToolBar->AddTool(ID_TOOL_CREATE_TABLE, wxT("创建表格模板"), wxBITMAP(IDB_CREATE_TABLE), wxT("创建表格模板"));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListBox(this, ID_TABLE_RESOURCE_LISTBOX, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxHSCROLL | wxVSCROLL);

	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	mResourceList->Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(SupportedTableWindow::OnRightDown), NULL, this);
	mResourceList->Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(SupportedTableWindow::OnRightUp), NULL, this);

	mConfigDataDialog = new ConfigDataDialog(this, wxT("创建表格"), wxDefaultPosition, wxSize(650, 650), wxDEFAULT_DIALOG_STYLE, wxT("ConfigDataDialog"));
	mConfigDataDialog->Show(false);
}

void SupportedTableWindow::OnShow(wxShowEvent& event)
{
	;
}

void SupportedTableWindow::refresh()
{
	mResourceList->Clear();
	DataBase* dataBase = mEditorFrame->getDataBase();
	const std::map<std::string, DataTemplate*>& templateList = dataBase->getDataTemplateList();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplate = templateList.begin();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplateEnd = templateList.end();
	for (; iterTemplate != iterTemplateEnd; ++iterTemplate)
	{
		mResourceList->Append(wxString(iterTemplate->first.c_str()));
	}
}

void SupportedTableWindow::OnRightUp(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	wxMenu menu;
	menu.Append(ID_NEW_DATA, wxT("创建新数据"));
	PopupMenu(&menu, clientpt);
	event.Skip();
}

void SupportedTableWindow::OnRightDown(wxMouseEvent& event)
{
	wxPoint clientpt = event.GetPosition();
	int index = mResourceList->HitTest(clientpt);
	mResourceList->SetSelection(index);
	event.Skip();
}

void SupportedTableWindow::OnNewData(wxCommandEvent& event)
{
	std::string itemName = mResourceList->GetStringSelection().ToStdString();
	if (itemName == "")
	{
		return;
	}
	DataBase* dataBase = mEditorFrame->getDataBase();
	if (!dataBase->newData(itemName))
	{
		return;
	}
	dataBase->writeBinaryFile(itemName);

	// 创建完成后刷新二进制资源列表
	mEditorFrame->getTableBinaryResourceWindow()->refresh();
}

void SupportedTableWindow::OnRefresh(wxCommandEvent& event)
{
	refresh();
}

void SupportedTableWindow::OnCreateTable(wxCommandEvent& event)
{
	editTable(NULL);
}

void SupportedTableWindow::editTable(DataTemplate* dataTemplate)
{
	mConfigDataDialog->Show();
	mConfigDataDialog->editTable(dataTemplate);
}