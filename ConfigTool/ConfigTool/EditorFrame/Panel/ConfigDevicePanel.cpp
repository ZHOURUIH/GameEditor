#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"

#include "ConfigDevicePanel.h"
#include "EditorUtility.h"

enum
{
	ID_CONFIRM_BUTTON,
};

BEGIN_EVENT_TABLE(ConfigDevicePanel, wxPanel)
EVT_BUTTON(ID_CONFIRM_BUTTON, OnConfirmButton)
END_EVENT_TABLE()

ConfigDevicePanel::ConfigDevicePanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

void ConfigDevicePanel::setup()
{
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxHORIZONTAL);

	mIPLabel = new wxStaticText(this, wxID_ANY, wxT("IP:"), wxDefaultPosition, wxDefaultSize, 0);
	mIPLabel->Wrap(-1);
	bSizer8->Add(mIPLabel, 0, wxALL, 5);

	mIPEditText = new wxTextCtrl(this, wxID_ANY, wxT("192.168.2.165"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer8->Add(mIPEditText, 0, wxALL, 5);

	bSizer7->Add(bSizer8, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	mMachineIndexLabel = new wxStaticText(this, wxID_ANY, wxT("»úÆ÷ºÅ:"), wxDefaultPosition, wxDefaultSize, 0);
	mMachineIndexLabel->Wrap(-1);
	bSizer9->Add(mMachineIndexLabel, 0, wxALL, 5);

	mMachineEditText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer9->Add(mMachineEditText, 0, wxALL, 5);

	bSizer7->Add(bSizer9, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	mConfirmButton = new wxButton(this, ID_CONFIRM_BUTTON, wxT("È·¶¨"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer10->Add(mConfirmButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	bSizer7->Add(bSizer10, 3, wxEXPAND, 5);

	bSizer5->Add(bSizer7, 2, wxEXPAND, 5);

	this->SetSizer(bSizer5);
	this->Layout();
}

void ConfigDevicePanel::OnConfirmButton(wxCommandEvent& event)
{
	wxString ipString = mIPEditText->GetValue();
	wxString machineIndexString = mMachineEditText->GetValue();
	mConfigToolCore->setDeviceParameter(ipString.ToStdString(), txUtility::stringToInt(machineIndexString.ToStdString()));
}