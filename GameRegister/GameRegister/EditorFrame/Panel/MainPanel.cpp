#include "txRegisterTool.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "MainPanel.h"

enum
{
	ID_GAME_NAME_COMBO,
	ID_REQUEST_TEXT,
	ID_REGISTER_BUTTON,
	ID_REGISTER_TEXT,
};

BEGIN_EVENT_TABLE(MainPanel, EditorPanel)
EVT_BUTTON(ID_REGISTER_BUTTON, onRegisterButton)
END_EVENT_TABLE()

MainPanel::MainPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

MainPanel::~MainPanel()
{
	;
}

void MainPanel::setup()
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer5 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* staticText3 = new wxStaticText(this, wxID_ANY, wxT("输入申请码,然后点击按钮获得注册码"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	bSizer5->Add(staticText3, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer1->Add(bSizer5, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL, 5);

	wxBoxSizer* bSizer6 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* staticText0 = new wxStaticText(this, wxID_ANY, wxT("游戏名称:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer6->Add(staticText0, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mEncodeKeyCombo = new wxComboBox(this, ID_GAME_NAME_COMBO, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	bSizer6->Add(mEncodeKeyCombo, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mEncodeKeyCombo->Append("电能动力4.3");
	mEncodeKeyCombo->Append("电能动力赛道版2.0");
	mEncodeKeyCombo->SetSelection(0);
	bSizer1->Add(bSizer6, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* staticText1 = new wxStaticText(this, wxID_ANY, wxT("输入申请码:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(staticText1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	mRequestText = new wxTextCtrl(this, ID_REQUEST_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mRequestText, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mRegisterButton = new wxButton(this, ID_REGISTER_BUTTON, wxT("获得注册码"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(mRegisterButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
	bSizer1->Add(bSizer2, 2, wxEXPAND, 5);

	wxBoxSizer* bSizer4 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* staticText2 = new wxStaticText(this, wxID_ANY, wxT("注册码:"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(staticText2, 0, wxALL, 5);
	mRegisterText = new wxTextCtrl(this, ID_REGISTER_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(mRegisterText, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	bSizer1->Add(bSizer4, 2, wxEXPAND, 5);

	this->SetSizer(bSizer1);
	this->Layout();
}

void MainPanel::onRegisterButton(wxCommandEvent& event)
{
	std::string encodeKey = mEncodeKeyCombo->GetValue();
	if (encodeKey == "")
	{
		EditorUtility::logInfo("请选择游戏名称!");
		return;
	}
	txRegisterTool registerTool;
	std::string requestCode = mRequestText->GetValue();
	if (requestCode == "")
	{
		EditorUtility::logInfo("请输入申请码!");
		return;
	}
	std::string newRegisteCode = registerTool.generateRegisteCode(requestCode, encodeKey);
	mRegisterText->SetValue(newRegisteCode);
}