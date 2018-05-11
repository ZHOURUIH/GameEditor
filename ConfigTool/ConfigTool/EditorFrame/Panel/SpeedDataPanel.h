#ifndef _SPEED_DATA_PANEL_H_
#define _SPEED_DATA_PANEL_H_

#include <map>
#include <vector>
#include <set>

#include "EditorPanel.h"
#include "CommonDefine.h"

class SpeedData;
class SpeedDataPanel : public EditorPanel
{
public:
	SpeedDataPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~SpeedDataPanel();
	void showData(std::map<int, SpeedData*>& dataList);
	void refresh();
	DECLARE_EVENT_TABLE()
	void OnFrictionChanged(wxCommandEvent& event);
	void OnConfirmColor(wxCommandEvent& event);
	void OnSpeedRatio(wxCommandEvent& event);
protected:
	void setup();
	void resetListItemCount(int count);
	void updateItem(const int& item, const int& column, const std::string& text);
protected:
	wxStaticText* mFrictionLabel;
	std::vector<wxRadioButton*> mFrictionRadioButtonList;
	wxListCtrl* mSpeedDataList;
	wxTextCtrl* mColorRed;
	wxTextCtrl* mColorGreen;
	wxTextCtrl* mColorBlue;
	wxButton* mConfirmColor;
	wxTextCtrl* mSpeedRatio;
	wxButton* mSpeedRatioButton;
};

#endif