#ifndef _HISTORY_DATE_PANEL_H_
#define _HISTORY_DATE_PANEL_H_

#include "EditorHeader.h"
#include "DataDefine.h"

class DataHistoryData;
class EditorFrame;
class HistoryDatePanel : public wxPanel
{
public:
	HistoryDatePanel(wxWindow* parent, long style);
	virtual ~HistoryDatePanel(){}
	void setup();
	void showHistoryDate();
	DECLARE_EVENT_TABLE()
	void OnItemClicked(wxDataViewEvent& event);
	void OnShow(wxShowEvent& event);
protected:
	void refreshHistroyList();
protected:
	wxAuiToolBar* mAuiToolBar;
	wxDataViewListCtrl* mDateListCtrl;
	EditorFrame* mEditorFrame;
	std::map<std::string, std::vector<DataHistoryData*> > mHistoryList;
};

#endif