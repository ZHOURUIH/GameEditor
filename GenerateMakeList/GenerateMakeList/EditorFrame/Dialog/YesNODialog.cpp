#include "YesNODialog.h"

enum
{
	ID_YES_BUTTON,
	ID_NO_BUTTON,
};

BEGIN_EVENT_TABLE(YesNODialog, wxDialog)
EVT_BUTTON(ID_YES_BUTTON, onYesButton)
EVT_BUTTON(ID_NO_BUTTON, onNoButton)
END_EVENT_TABLE()

void YesNODialog::setup()
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	mInfoText = new wxStaticText(this, wxID_ANY, wxT("提示信息"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	mInfoText->Wrap(-1);
	bSizer2->Add(mInfoText, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);

	bSizer1->Add(bSizer2, 3, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxVERTICAL);
	mYesButton = new wxButton(this, ID_YES_BUTTON, wxT("是"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(mYesButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	bSizer3->Add(bSizer4, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxVERTICAL);
	mNoButton = new wxButton(this, ID_NO_BUTTON, wxT("否"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mNoButton, 0, wxALL, 5);
	bSizer3->Add(bSizer5, 1, wxEXPAND, 5);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);
}

void YesNODialog::setInfo(const std::string& info)
{
	mInfoText->SetLabel(info);
}

void YesNODialog::onYesButton(wxCommandEvent& event)
{
	EndModal(wxYES);
}

void YesNODialog::onNoButton(wxCommandEvent& event)
{
	EndModal(wxNO);
}