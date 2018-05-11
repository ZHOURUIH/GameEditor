#ifndef _DEVICE_LIST_PANEL_H_
#define _DEVICE_LIST_PANEL_H_

#include <vector>

#include "EditorPanel.h"

class DeviceListPanel : public EditorPanel
{
public:
	DeviceListPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	virtual ~DeviceListPanel(){}
	void refreshDeviceList(std::vector<std::pair<std::string, std::string> >& deviceList);
	void setReceiveData(char* data, const int& dataCount);
	void setSendData(char* data, const int& dataCount);
	DECLARE_EVENT_TABLE()
	void OnDeviceSelected(wxCommandEvent& event);
protected:
	void setup();
protected:
	wxTextCtrl* mReceiveHex;
	wxTextCtrl* mReceiveString;
	wxTextCtrl* mSendTextHex;
	wxTextCtrl* mSendTextString;
	wxComboBox* mDeviceListCombo;
};

#endif