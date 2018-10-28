#ifndef _HEART_RATE_PANEL_H_
#define _HEART_RATE_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class HeartRateData;
class HeartRatePanel : public EditorPanel
{
public:
	HeartRatePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~HeartRatePanel(){}
	void init();
	void update(float elapsedTime);
	void refreshTime();
	DECLARE_EVENT_TABLE()
	void OnConfirmTimeOut(wxCommandEvent& e);
protected:
	void setup();
	void showData(txMap<std::string, HeartRateData*>& dataList);
protected:
	wxListCtrl* mHeartRateDataList;
	wxTextCtrl* mHeartRateTimeOutEdit;
	wxButton* mConfirmTimeOut;
	wxStaticText* mCurHeartRateTimeOut;
};

#endif