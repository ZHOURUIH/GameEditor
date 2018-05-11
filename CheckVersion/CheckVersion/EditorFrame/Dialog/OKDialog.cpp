#include "OKDialog.h"

enum
{
	ID_OK_BUTTON,
};

BEGIN_EVENT_TABLE(OKDialog, wxDialog)
EVT_BUTTON(ID_OK_BUTTON, onOKButton)
END_EVENT_TABLE()

void OKDialog::setup()
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	mInfoText = new wxStaticText(this, wxID_ANY, wxT("提示信息"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	mInfoText->Wrap(-1);
	bSizer2->Add(mInfoText, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 5);
	bSizer1->Add(bSizer2, 3, wxEXPAND, 5);

	wxBoxSizer* bSizer3 = new wxBoxSizer(wxVERTICAL);
	mOKButton = new wxButton(this, ID_OK_BUTTON, wxT("确定"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mOKButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
	this->Centre(wxBOTH);
}

void OKDialog::setInfo(const std::string& info)
{
	mInfoText->SetLabel(info);
}

void OKDialog::onOKButton(wxCommandEvent& event)
{
	EndModal(wxOK);
}