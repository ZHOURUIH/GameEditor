#ifndef _DEVICE_REGISTE_PANEL_H_
#define _DEVICE_REGISTE_PANEL_H_

#include "EditorPanel.h"

class DeviceRegistePanel : public EditorPanel
{
public:
	DeviceRegistePanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~DeviceRegistePanel();
	void setup();
	void refreshState();
	void onRegisteResult(int ret);
	DECLARE_EVENT_TABLE()
	void onRegisteButton(wxCommandEvent& event);
protected:
	wxStaticText* mGymNameLabel;
	wxTextCtrl* mGymNameEdit;
	wxStaticText* mDeviceCodeLabel;
	wxTextCtrl* mDeviceCodeEdit;
	wxButton* mConfirm;
	wxStaticText* mTipLabel;
};

#endif