#include "EditorApp.h"
#include "EditorFrame.h"
#include "TableTxtResourceWindow.h"
#include "DataDefine.h"
#include "DataBase.h"
#include "MainViewerWindow.h"
#include "TableBinaryResourceWindow.h"
#include "txUtility.h"

enum
{
	ID_REFRESH,
	ID_TABLE_RESOURCE_LISTBOX,
};

BEGIN_EVENT_TABLE(TableTxtResourceWindow, wxPanel)

EVT_TOOL(ID_REFRESH, TableTxtResourceWindow::OnRefresh)
EVT_LISTBOX_DCLICK(ID_TABLE_RESOURCE_LISTBOX, TableTxtResourceWindow::OnDoubleClick)
EVT_SHOW(TableTxtResourceWindow::OnShow)

END_EVENT_TABLE()

TableTxtResourceWindow::TableTxtResourceWindow(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
wxPanel(parent, wxID_ANY, position, size, style, name),
mToolBar(NULL)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

TableTxtResourceWindow::~TableTxtResourceWindow()
{
	;
}

void TableTxtResourceWindow::setup()
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

void TableTxtResourceWindow::OnShow(wxShowEvent& event)
{
	;
}

void TableTxtResourceWindow::refresh()
{
	mResourceList->Clear();
	DataBase* dataBase = mEditorFrame->getDataBase();
	const std::map<std::string, DataTemplate*>& templateList = dataBase->getDataTemplateList();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplate = templateList.begin();
	std::map<std::string, DataTemplate*>::const_iterator iterTemplateEnd = templateList.end();
	for (; iterTemplate != iterTemplateEnd; ++iterTemplate)
	{
		if (txUtility::isFileExist(txUtility::getAvailableResourcePath(GAME_DATA_PATH + iterTemplate->first + TEMPLATE_SUFFIX)))
		{
			mResourceList->Append(wxString(iterTemplate->first.c_str()));
		}
	}
}

void TableTxtResourceWindow::OnRefresh(wxCommandEvent& event)
{
	refresh();
}

void TableTxtResourceWindow::OnDoubleClick(wxCommandEvent& event)
{
	DataBase* dataBase = mEditorFrame->getDataBase();
	std::string itemName = event.GetString().ToStdString();
	// 转化为二进制表格
	dataBase->txtToBinary(itemName + TEMPLATE_SUFFIX);
	// 刷新二进制表格资源框
	TableBinaryResourceWindow* binaryResource = mEditorFrame->getTableBinaryResourceWindow();
	binaryResource->refresh();
	// 加载二进制表格
	dataBase->loadData(txUtility::getAvailableResourcePath(GAME_DATA_PATH + itemName + DATA_SUFFIX), true, true);
	// 显示表格数据
	MainViewerWindow* mainViewer = mEditorFrame->getMainViewerWindow();
	mainViewer->showData(itemName);
}