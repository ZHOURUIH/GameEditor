#ifndef _CONFIG_DEVICE_PANEL_H_
#define _CONFIG_DEVICE_PANEL_H_

#include <map>
#include <vector>
#include <set>

#include "EditorPanel.h"
#include "CommonDefine.h"

class ConfigDevicePanel : public EditorPanel
{
public:
	ConfigDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	virtual ~ConfigDevicePanel(){}
	DECLARE_EVENT_TABLE()
	void OnConfirmButton(wxCommandEvent& event);
protected:
	void setup();
protected:
	wxStaticText* mIPLabel;
	wxTextCtrl* mIPEditText;
	wxStaticText* mMachineIndexLabel;
	wxTextCtrl* mMachineEditText;
	wxButton* mConfirmButton;
};

#endif