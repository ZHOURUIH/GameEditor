#ifndef _OK_DIALOG_H_
#define _OK_DIALOG_H_

#include "EditorDialog.h"

class OKDialog : public EditorDialog
{
public:
	OKDialog(const wxString& title, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxSize(375, 151), long style = wxDEFAULT_DIALOG_STYLE)
		:
		EditorDialog(NULL, title, position, size, style, title)
	{
		setup();
	}
	void setup();
	void setInfo(const std::string& info);
	DECLARE_EVENT_TABLE()
	void onOKButton(wxCommandEvent& event);
protected:
	wxStaticText* mInfoText;
	wxButton* mOKButton;
};

#endif