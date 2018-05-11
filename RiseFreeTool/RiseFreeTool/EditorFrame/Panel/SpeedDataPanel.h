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
	void notifyDeviceListChanged();
	void notifyCurDeviceChanged(const std::string& device);
	void notifyDeviceOpen(bool open);
	DECLARE_EVENT_TABLE()
	void OnListenButton(wxCommandEvent& event);
	void OnCancelListenButton(wxCommandEvent& event);
	void OnFrictionChanged(wxCommandEvent& event);
protected:
	void setup();
	void updateItem(const int& item, const int& column, const std::string& text);
	void resetListItemCount(int count);
protected:
	wxComboBox* mDeviceListCombo;
	wxListCtrl* mDataListCtrl;
	wxButton* mListenButton;
	wxButton* mCancelListenButton;
	txVector<wxRadioButton*> mFrictionRadioButtonList;
};

#endif