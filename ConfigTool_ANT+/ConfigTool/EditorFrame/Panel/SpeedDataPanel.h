#ifndef _SPEED_DATA_PANEL_H_
#define _SPEED_DATA_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"

class SpeedData;
class SpeedDataPanel : public EditorPanel
{
public:
	SpeedDataPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~SpeedDataPanel();
	void init();
	void update(float elapsedTime);
	void refresh();
	DECLARE_EVENT_TABLE()
	void OnFrictionChanged(wxCommandEvent& event);
	void OnConfirmColor(wxCommandEvent& event);
	void OnSpeedRatio(wxCommandEvent& event);
protected:
	void setup();
	void showData(txMap<int, SpeedData*>& dataList);
protected:
	wxStaticText* mFrictionLabel;
	txVector<wxRadioButton*> mFrictionRadioButtonList;
	wxListCtrl* mSpeedDataList;
	wxTextCtrl* mColorRed;
	wxTextCtrl* mColorGreen;
	wxTextCtrl* mColorBlue;
	wxButton* mConfirmColor;
	wxTextCtrl* mSpeedRatio;
	wxButton* mSpeedRatioButton;
};

#endif