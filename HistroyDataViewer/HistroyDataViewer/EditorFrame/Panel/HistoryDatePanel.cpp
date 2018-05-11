#include "EditorApp.h"
#include "EditorFrame.h"
#include "HistoryDatePanel.h"
#include "EditorUtility.h"
#include "DataHistoryData.h"
#include "DataBase.h"
#include "MainViewerPanel.h"
#include "txUtility.h"

enum
{
	ID_DATE_LIST,
};

BEGIN_EVENT_TABLE(HistoryDatePanel, wxPanel)
EVT_DATAVIEW_SELECTION_CHANGED(ID_DATE_LIST, OnItemClicked)
EVT_SHOW(OnShow)
END_EVENT_TABLE()

HistoryDatePanel::HistoryDatePanel(wxWindow* parent, long style)
:
wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
{
	mEditorFrame = wxGetApp().GetMainFrame();
	setup();
}

void HistoryDatePanel::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	mAuiToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	mAuiToolBar->SetToolBitmapSize(wxSize(16, 16));
	mAuiToolBar->Realize();

	bSizer1->Add(mAuiToolBar, 0, wxALL, 5);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	mDateListCtrl = new wxDataViewListCtrl(this, ID_DATE_LIST, wxDefaultPosition, wxSize(-1, -1), wxDV_HORIZ_RULES | wxDV_VERT_RULES | wxDV_MULTIPLE);
	bSizer2->Add(mDateListCtrl, 1, wxALL | wxEXPAND, 5);

	mDateListCtrl->AppendTextColumn("Ê±¼ä", wxDATAVIEW_CELL_EDITABLE, 250);

	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void HistoryDatePanel::showHistoryDate()
{
	refreshHistroyList();
	mDateListCtrl->DeleteAllItems();
	wxVector<wxVariant> valueList;
	valueList.push_back("");
	std::map<std::string, std::vector<DataHistoryData*> >::iterator iterDate = mHistoryList.begin();
	std::map<std::string, std::vector<DataHistoryData*> >::iterator iterDateEnd = mHistoryList.end();
	for (; iterDate != iterDateEnd; ++iterDate)
	{
		valueList[0] = iterDate->first;
		mDateListCtrl->AppendItem(valueList);
	}
}

void HistoryDatePanel::OnItemClicked(wxDataViewEvent& event)
{
	wxDataViewItemArray itemArray;
	int count = mDateListCtrl->GetSelections(itemArray);
	if (count > 0)
	{
		int firstItem = mDateListCtrl->ItemToRow(itemArray[0]);
		std::string date = mDateListCtrl->GetTextValue(firstItem, 0);
		std::map<std::string, std::vector<DataHistoryData*> >::iterator iterHistory = mHistoryList.find(date);
		if (iterHistory != mHistoryList.end())
		{
			mEditorFrame->getMainViewerPanel()->showData(iterHistory->second);
		}
	}
}

void HistoryDatePanel::OnShow(wxShowEvent& event)
{
	mEditorFrame->notifyWindowShow(this, event.IsShown());
}

void HistoryDatePanel::refreshHistroyList()
{
	DATA_TYPE type = DT_HISTORY_DATA;
	DataBase* dataBase = mEditorFrame->getDataBase();

	mHistoryList.clear();
	int dataCount = dataBase->getDataCount(type);
	for (int i = 0; i < dataCount; ++i)
	{
		DataHistoryData* data = static_cast<DataHistoryData*>(dataBase->queryData(type, i));
		std::map<std::string, std::vector<DataHistoryData*> >::iterator iterHistory = mHistoryList.find(data->mDate);
		if (iterHistory != mHistoryList.end())
		{
			iterHistory->second.push_back(data);
		}
		else
		{
			std::vector<DataHistoryData*> dataList;
			dataList.push_back(data);
			mHistoryList.insert(std::make_pair(data->mDate, dataList));
		}
	}
}