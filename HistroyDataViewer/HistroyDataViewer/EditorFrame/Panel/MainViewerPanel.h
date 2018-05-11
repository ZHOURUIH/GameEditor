#ifndef _MAIN_VIEWER_PANEL_H_
#define _MAIN_VIEWER_PANEL_H_

#include "EditorHeader.h"
#include "DataDefine.h"

class DataHistoryData;
class MainViewerPanel : public wxPanel
{
public:
	MainViewerPanel(wxWindow* parent, long style);
	virtual ~MainViewerPanel(){}
	void setup();
	void showData(const std::vector<DataHistoryData*>& dataList);
	DECLARE_EVENT_TABLE()
	void OnShow(wxShowEvent& event);
protected:
	const std::string& getItemName(const std::string& dataName)
	{
		std::map<std::string, std::string>::iterator iter = mItemNames.find(dataName);
		if (iter != mItemNames.end())
		{
			return iter->second;
		}
		return dataName;
	}
protected:
	wxAuiToolBar* mAuiToolBar;
	wxDataViewListCtrl* mDataInfoListCtrl;
	EditorFrame* mEditorFrame;
	std::map<std::string, std::string> mItemNames;
};

#endif