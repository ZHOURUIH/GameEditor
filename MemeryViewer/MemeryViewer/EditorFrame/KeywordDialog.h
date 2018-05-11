#ifndef _KEYWORD_DIALOG_H_
#define _KEYWORD_DIALOG_H_

#include <set>
#include <string>

#include "EditorHeader.h"

class KeywordDialog : public wxDialog
{
public:
	KeywordDialog(wxString title);
	~KeywordDialog();
	void init();
	std::set<std::string>& getKeywordList() { return mKeywordList; }
	void addKeyword(std::string keyword);
	DECLARE_EVENT_TABLE()
	void onTextReturn(wxCommandEvent& event);
	void onAddKeyword(wxCommandEvent& event);
	void onDeleteKeyword(wxCommandEvent& event);
protected:
	wxTextCtrl* mText;
	wxButton* mAddButton;
	wxButton* mDeleteButton;
	wxListBox* mKeywordListBox;
	std::set<std::string> mKeywordList;
};

#endif