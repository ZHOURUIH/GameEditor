#include "txUtility.h"

#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"

#include "DeviceListPanel.h"

enum
{
	ID_DEVICE_LIST_COMBO,
};

BEGIN_EVENT_TABLE(DeviceListPanel, wxPanel)
EVT_COMBOBOX(ID_DEVICE_LIST_COMBO, OnDeviceSelected)
END_EVENT_TABLE()

DeviceListPanel::DeviceListPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void DeviceListPanel::setup()
{
	wxBoxSizer* bSizer6 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer7 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bSizer12 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* receiveLabel = new wxStaticText(this, wxID_ANY, wxT("接收:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer12->Add(receiveLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mReceiveHex = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer12->Add(mReceiveHex, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer7->Add(bSizer12, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer10 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* receiveStringLabel = new wxStaticText(this, wxID_ANY, wxT("接收:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer10->Add(receiveStringLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mReceiveString = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer10->Add(mReceiveString, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer7->Add(bSizer10, 1, wxEXPAND, 5);
	bSizer6->Add(bSizer7, 1, wxEXPAND, 5);
	
	wxBoxSizer* bSizer8 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bSizer13 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* sendLabel = new wxStaticText(this, wxID_ANY, wxT("发送:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer13->Add(sendLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mSendTextHex = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer13->Add(mSendTextHex, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer8->Add(bSizer13, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer11 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* sendStringLabel = new wxStaticText(this, wxID_ANY, wxT("发送:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer11->Add(sendStringLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mSendTextString = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer11->Add(mSendTextString, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer8->Add(bSizer11, 1, wxEXPAND, 5);
	bSizer6->Add(bSizer8, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer9 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* deviceListLabel = new wxStaticText(this, wxID_ANY, wxT("设备列表:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer9->Add(deviceListLabel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mDeviceListCombo = new wxComboBox(this, ID_DEVICE_LIST_COMBO, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	bSizer9->Add(mDeviceListCombo, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer6->Add(bSizer9, 2, wxEXPAND, 5);

	this->SetSizer(bSizer6);
	this->Layout();
}

void DeviceListPanel::refreshDeviceList(std::vector<std::pair<std::string, std::string> >& deviceList)
{
	mDeviceListCombo->Clear();
	int deviceCount = deviceList.size();
	for (int i = 0; i < deviceCount; ++i)
	{
		mDeviceListCombo->Append(deviceList[i].second.c_str());
	}
}

void DeviceListPanel::OnDeviceSelected(wxCommandEvent& event)
{
	wxString deviceName = mDeviceListCombo->GetValue();
	mConfigToolCore->setCurDeviceName(deviceName.ToStdString());
}

void DeviceListPanel::setReceiveData(char* data, const int& dataCount)
{
	std::string dataStr = txUtility::charArrayToHexString((unsigned char*)data, dataCount);
	mReceiveHex->SetValue(dataStr);
	mReceiveString->SetValue(data);
}

void DeviceListPanel::setSendData(char* data, const int& dataCount)
{
	std::string dataStr = txUtility::charArrayToHexString((unsigned char*)data, dataCount);
	mSendTextHex->SetValue(dataStr);
	mSendTextString->SetValue(data);
}