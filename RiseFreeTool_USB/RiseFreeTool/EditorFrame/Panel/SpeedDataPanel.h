#ifndef _SPEED_DATA_PANEL_H_
#define _SPEED_DATA_PANEL_H_

#include "EditorPanel.h"
#include "CommonDefine.h"
#include "DeviceData.h"

class SpeedDataPanel : public EditorPanel
{
public:
	SpeedDataPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~SpeedDataPanel();
	void update();
	DECLARE_EVENT_TABLE()
	void OnFrictionChanged(wxCommandEvent& event);
protected:
	void setup();
	void updateItem(const int& item, const int& column, const std::string& text);
	void resetListItemCount(int count);
protected:
	wxListCtrl* mDataListCtrl;
	txVector<wxRadioButton*> mFrictionRadioButtonList;
};

#endif