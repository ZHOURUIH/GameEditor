#include "KeywordDialog.h"

enum
{
	ID_TEXT,
	ID_ADD_BUTTON,
	ID_DELETE_BUTTON,
	ID_KEYWORD_LIST,
};

BEGIN_EVENT_TABLE(KeywordDialog, wxDialog)

EVT_TEXT_ENTER(ID_TEXT, onTextReturn)
EVT_BUTTON(ID_ADD_BUTTON, onAddKeyword)
EVT_BUTTON(ID_DELETE_BUTTON, onDeleteKeyword)

END_EVENT_TABLE()

KeywordDialog::KeywordDialog(wxString title)
:
wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(-1, 400))
{}

KeywordDialog::~KeywordDialog()
{
	;
}

void KeywordDialog::init()
{
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	mText = new wxTextCtrl(this, ID_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mText, 1, wxALL | wxEXPAND, 5);

	mAddButton = new wxButton(this, ID_ADD_BUTTON, wxT("添加"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mAddButton, 0, wxALL, 5);

	mDeleteButton = new wxButton(this, ID_DELETE_BUTTON, wxT("删除"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(mDeleteButton, 0, wxALL, 5);

	bSizer4->Add(bSizer5, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);

	mKeywordListBox = new wxListBox(this, ID_KEYWORD_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	bSizer6->Add(mKeywordListBox, 1, wxALL | wxEXPAND, 5);

	bSizer4->Add(bSizer6, 1, wxEXPAND, 5);

	this->SetSizer(bSizer4);
	this->Layout();
}

void KeywordDialog::addKeyword(std::string keyword)
{
	std::set<std::string>::iterator iter = mKeywordList.find(keyword);
	// 如果已经存在,则不能在添加
	if (iter != mKeywordList.end())
	{
		return;
	}
	// 列表框中的顺序与mKeywordList中的顺序不同
	mKeywordListBox->Insert(keyword, mKeywordListBox->GetCount());
	mKeywordList.insert(keyword);
}

void KeywordDialog::onAddKeyword(wxCommandEvent& event)
{
	wxString text = mText->GetValue();
	if (text != "")
	{
		addKeyword(text.ToStdString());
	}
	event.Skip();
}

void KeywordDialog::onTextReturn(wxCommandEvent& event)
{
	wxString text = mText->GetValue();
	if (text != "")
	{
		addKeyword(text.ToStdString());
	}
	event.Skip();
}

void KeywordDialog::onDeleteKeyword(wxCommandEvent& event)
{
	// 删除选中项
	int sel = mKeywordListBox->GetSelection();
	if (sel != -1)
	{
		wxString text = mKeywordListBox->GetString(sel);
		std::set<std::string>::iterator iter = mKeywordList.find(text.ToStdString());
		if (iter != mKeywordList.end())
		{
			mKeywordList.erase(iter);
		}
		mKeywordListBox->Delete(sel);
	}
	event.Skip();
}