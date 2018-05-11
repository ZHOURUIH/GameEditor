#ifndef _DEVICE_INFO_PANEL_H_
#define _DEVICE_INFO_PANEL_H_

#include "EditorPanel.h"

class DeviceInfoPanel : public EditorPanel
{
public:
	DeviceInfoPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~DeviceInfoPanel();
	void setup();
	void refreshInfo();
	DECLARE_EVENT_TABLE()
protected:
	wxStaticText* mRegisteState;
	wxStaticText* mGymNameLabel;
	wxStaticText* mGymNameText;
	wxStaticText* mDeviceCodeLabel;
	wxStaticText* mDeviceCodeText;
	wxStaticText* mRegisteCodeLabel;
	wxStaticText* mRegisteCodeText;
	wxStaticText* mRegisteDateLabel;
	wxStaticText* mRegisteDateText;
};

#endif