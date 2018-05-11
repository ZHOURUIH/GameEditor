#ifndef _MAIN_PANEL_H_
#define _MAIN_PANEL_H_

#include "EditorPanel.h"

class MainPanel : public EditorPanel
{
public:
	MainPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	~MainPanel();
	void setup();
	DECLARE_EVENT_TABLE()
	void onRegisterButton(wxCommandEvent& event);
protected:
	wxTextCtrl* mRequestText;
	wxComboBox* mEncodeKeyCombo;
	wxButton* mRegisterButton;
	wxTextCtrl* mRegisterText;
};

#endif