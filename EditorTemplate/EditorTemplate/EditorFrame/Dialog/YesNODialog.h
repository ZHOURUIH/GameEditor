#ifndef _YES_NO_DIALOG_H_
#define _YES_NO_DIALOG_H_

#include "EditorDialog.h"

class YesNODialog : public EditorDialog
{
public:
	YesNODialog(const wxString& title, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxSize(375, 151), long style = wxDEFAULT_DIALOG_STYLE)
		:
		EditorDialog(NULL, title, position, size, style, title)
	{
		setup();
	}
	void setup();
	void setInfo(const std::string& info);
	DECLARE_EVENT_TABLE()
	void onYesButton(wxCommandEvent& event);
	void onNoButton(wxCommandEvent& event);
protected:
	wxStaticText* mInfoText;
	wxButton* mYesButton;
	wxButton* mNoButton;
};

#endif