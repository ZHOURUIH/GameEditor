#include "EditorApp.h"
#include "EditorFrame.h"
#include "TableBinaryResourceWindow.h"
#include "DataDefine.h"
#include "DataBase.h"
#include "MainViewerWindow.h"
#include "txUtility.h"

enum
{
	ID_REFRESH,
	ID_TABLE_RESOURCE_LISTBOX,
};

BEGIN_EVENT_TABLE(TableBinaryResourceWindow, wxPanel)

EVT_TOOL(ID_REFRESH, TableBinaryResourceWindow::OnRefresh)
EVT_LISTBOX_DCLICK(ID_TABLE_RESOURCE_LISTBOX, TableBinaryResourceWindow::OnDoubleClick)
EVT_SHOW(TableBinaryResourceWindow::OnShow)

END_EVENT_TABLE()

TableBinaryResourceWindow::TableBinaryResourceWindow(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
wxPanel(parent, wxID_ANY, position, size, style, name),
mToolBar(NULL)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

TableBinaryResourceWindow::~TableBinaryResourceWindow()
{
	;
}

void TableBinaryResourceWindow::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mToolBar->SetToolBitmapSize(wxSize(16, 16));
	mToolBar->AddTool(ID_REFRESH, "刷新资源", wxBITMAP(IDB_REFRESH));
	mToolBar->Realize();

	bSizer1->Add(mToolBar, 0, wxALL, 5);

	mResourceList = new wxListBox(this, ID_TABLE_RESOURCE_LISTBOX, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxHSCROLL | wxVSCROLL);

	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void TableBinaryResourceWindow::OnShow(wxShowEvent& event)
{
	;
}

void TableBinaryResourceWindow::refresh()
{
	mResourceList->Clear();
	DataBase* dataBase = mEditorFrame->getDataBase();
	const std::map<std::string, DataTemplate*>& templateList = dataBase->getDataTemplateList();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplate = templateList.begin();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplateEnd = templateList.end();
	for (; iterTemplate != iterTemplateEnd; ++iterTemplate)
	{
		if (txUtility::isFileExist(txUtility::getAvailableResourcePath(GAME_DATA_PATH + iterTemplate->first + DATA_SUFFIX)))
		{
			mResourceList->Append(wxString(iterTemplate->first.c_str()));
		}
	}
}

void TableBinaryResourceWindow::OnRefresh(wxCommandEvent& event)
{
	refresh();
}

void TableBinaryResourceWindow::OnDoubleClick(wxCommandEvent& event)
{
	if (mEditorFrame->isModified())
	{
		int ret = wxMessageBox(wxT("是否保存当前表格?"), "提示", wxCANCEL | wxYES | wxNO);
		if (ret == wxYES)
		{
			mEditorFrame->saveData();
		}
		else if (ret == wxNO)
		{
			mEditorFrame->setDataModified(false);
		}
		else if (ret == wxCANCEL)
		{
			return;
		}
	}
	MainViewerWindow* mainViewer = mEditorFrame->getMainViewerWindow();
	std::string itemName = event.GetString().ToStdString();
	DataBase* dataBase = mEditorFrame->getDataBase();
	dataBase->loadData(txUtility::getAvailableResourcePath(GAME_DATA_PATH + itemName + DATA_SUFFIX), true, true);
	mainViewer->showData(itemName);
}