#include "DeviceInfoPanel.h"
#include "GymInfo.h"
#include "EditorCore.h"

BEGIN_EVENT_TABLE(DeviceInfoPanel, EditorPanel)
END_EVENT_TABLE()


DeviceInfoPanel::DeviceInfoPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

DeviceInfoPanel::~DeviceInfoPanel()
{
	;
}

void DeviceInfoPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	mRegisteState = new wxStaticText(this, wxID_ANY, wxT("设备已注册"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	mRegisteState->Wrap(-1);
	mRegisteState->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer2->Add(mRegisteState, 1, wxALL, 5);
	bSizer1->Add(bSizer2, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer10 = new wxBoxSizer(wxVERTICAL);
	mGymNameLabel = new wxStaticText(this, wxID_ANY, wxT("健身房名称:"), wxDefaultPosition, wxDefaultSize, 0);
	mGymNameLabel->Wrap(-1);
	mGymNameLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer10->Add(mGymNameLabel, 0, wxALIGN_RIGHT | wxALL, 5);
	bSizer3->Add(bSizer10, 1, wxEXPAND, 5);
	wxBoxSizer* bSizer11 = new wxBoxSizer(wxVERTICAL);
	mGymNameText = new wxStaticText(this, wxID_ANY, wxT("测试健身房"), wxDefaultPosition, wxDefaultSize, 0);
	mGymNameText->Wrap(-1);
	mGymNameText->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer11->Add(mGymNameText, 0, wxALL, 5);
	bSizer3->Add(bSizer11, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer12 = new wxBoxSizer(wxVERTICAL);
	mDeviceCodeLabel = new wxStaticText(this, wxID_ANY, wxT("设备码:"), wxDefaultPosition, wxDefaultSize, 0);
	mDeviceCodeLabel->Wrap(-1);
	mDeviceCodeLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer12->Add(mDeviceCodeLabel, 0, wxALL | wxALIGN_RIGHT, 5);
	bSizer4->Add(bSizer12, 1, wxEXPAND, 5);
	wxBoxSizer* bSizer13 = new wxBoxSizer(wxVERTICAL);
	mDeviceCodeText = new wxStaticText(this, wxID_ANY, wxT("121312312312312313"), wxDefaultPosition, wxDefaultSize, 0);
	mDeviceCodeText->Wrap(-1);
	mDeviceCodeText->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer13->Add(mDeviceCodeText, 0, wxALL, 5);
	bSizer4->Add(bSizer13, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer14 = new wxBoxSizer(wxVERTICAL);
	mRegisteCodeLabel = new wxStaticText(this, wxID_ANY, wxT("注册码:"), wxDefaultPosition, wxDefaultSize, 0);
	mRegisteCodeLabel->Wrap(-1);
	mRegisteCodeLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer14->Add(mRegisteCodeLabel, 0, wxALL | wxALIGN_RIGHT, 5);
	bSizer5->Add(bSizer14, 1, wxEXPAND, 5);
	wxBoxSizer* bSizer15 = new wxBoxSizer(wxVERTICAL);
	mRegisteCodeText = new wxStaticText(this, wxID_ANY, wxT("456456456456464131"), wxDefaultPosition, wxDefaultSize, 0);
	mRegisteCodeText->Wrap(-1);
	mRegisteCodeText->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer15->Add(mRegisteCodeText, 0, wxALL, 5);
	bSizer5->Add(bSizer15, 1, wxEXPAND, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer51 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* bSizer16 = new wxBoxSizer(wxVERTICAL);
	mRegisteDateLabel = new wxStaticText(this, wxID_ANY, wxT("注册日期:"), wxDefaultPosition, wxDefaultSize, 0);
	mRegisteDateLabel->Wrap(-1);
	mRegisteDateLabel->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer16->Add(mRegisteDateLabel, 0, wxALL | wxALIGN_RIGHT, 5);
	bSizer51->Add(bSizer16, 1, wxEXPAND, 5);
	wxBoxSizer* bSizer17 = new wxBoxSizer(wxVERTICAL);
	mRegisteDateText = new wxStaticText(this, wxID_ANY, wxT("2017.9.8 10:21:22"), wxDefaultPosition, wxDefaultSize, 0);
	mRegisteDateText->Wrap(-1);
	mRegisteDateText->SetFont(wxFont(20, 70, 90, 90, false, wxEmptyString));
	bSizer17->Add(mRegisteDateText, 0, wxALL, 5);
	bSizer51->Add(bSizer17, 1, wxEXPAND, 5);

	bSizer1->Add(bSizer51, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void DeviceInfoPanel::refreshInfo()
{
	GymInfo* gymInfo = mEditorCore->getGymInfo();
	mRegisteState->SetLabel(gymInfo->mRegisted ? "设备已注册" : "设备未注册");
	mRegisteState->SetForegroundColour(gymInfo->mRegisted ? wxColour(0, 255, 0) : wxColour(255, 0, 0));
	mGymNameText->SetLabel(gymInfo->mGymName);
	mDeviceCodeText->SetLabel(gymInfo->mDeviceCode);
	mRegisteCodeText->SetLabel(gymInfo->mRegisteCode);
	mRegisteDateText->SetLabel(gymInfo->mRegisteDate);
}