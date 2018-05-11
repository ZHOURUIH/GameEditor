#ifndef _HEART_RATE_PANEL_H_
#define _HEART_RATE_PANEL_H_

#include <map>
#include <vector>
#include <set>

#include "EditorPanel.h"
#include "CommonDefine.h"

class HeartRateData;
class HeartRatePanel : public EditorPanel
{
public:
	HeartRatePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~HeartRatePanel();
	void showData(std::map<std::string, HeartRateData*>& dataList);
	void refreshTime();
	DECLARE_EVENT_TABLE()
	void OnConfirmTimeOut(wxCommandEvent& e);
protected:
	void setup();
	void resetListItemCount(int count);
	void updateItem(const int& item, const int& column, const std::string& text);
protected:
	wxListCtrl* mHeartRateDataList;
	wxTextCtrl* mHeartRateTimeOutEdit;
	wxButton* mConfirmTimeOut;
	wxStaticText* mCurHeartRateTimeOut;
};

#endif