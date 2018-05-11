#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"

#include "ConfigCardPanel.h"
#include "EditorUtility.h"

enum
{
	ID_CONFIRM_BUTTON,
};

BEGIN_EVENT_TABLE(ConfigCardPanel, wxPanel)
EVT_BUTTON(ID_CONFIRM_BUTTON, OnConfirmButton)
END_EVENT_TABLE()

ConfigCardPanel::ConfigCardPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

ConfigCardPanel::~ConfigCardPanel()
{
	;
}

void ConfigCardPanel::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	mCardNameLabel = new wxStaticText(this, wxID_ANY, wxT("Ãû×Ö:"), wxDefaultPosition, wxDefaultSize, 0);
	mCardNameLabel->Wrap(-1);
	bSizer3->Add(mCardNameLabel, 0, wxALL | wxALIGN_BOTTOM, 5);

	mCardNameEditText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(mCardNameEditText, 1, wxALL | wxALIGN_BOTTOM, 5);

	bSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	mConfirmButton = new wxButton(this, ID_CONFIRM_BUTTON, wxT("È·¶¨"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(mConfirmButton, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);

	bSizer1->Add(bSizer4, 1, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void ConfigCardPanel::OnConfirmButton(wxCommandEvent& event)
{
	wxString text = mCardNameEditText->GetValue();
	mConfigToolCore->setCardParameter(text.ToStdString());
}