#ifndef _CONFIG_CARD_PANEL_H_
#define _CONFIG_CARD_PANEL_H_

#include <map>
#include <vector>
#include <set>

#include "EditorPanel.h"
#include "CommonDefine.h"

class ConfigCardPanel : public EditorPanel
{
public:
	ConfigCardPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name);
	~ConfigCardPanel();
	DECLARE_EVENT_TABLE()
	void OnConfirmButton(wxCommandEvent& event);
protected:
	void setup();
protected:
	wxStaticText* mCardNameLabel;
	wxTextCtrl* mCardNameEditText;
	wxButton* mConfirmButton;
};

#endif