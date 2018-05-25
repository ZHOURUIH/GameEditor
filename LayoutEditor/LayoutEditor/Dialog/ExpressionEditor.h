#ifndef _EXPRESSION_EDITOR_H_
#define _EXPRESSION_EDITOR_H_

#include "EditorDialog.h"

class PropertyWindow;
class ExpressionEditor : public EditorDialog
{
public:
	ExpressionEditor(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ExpressionEditor() {}
	void Setup();
	DECLARE_EVENT_TABLE()
	void OnOKButton(wxCommandEvent& event);
	void OnCancelButton(wxCommandEvent& event);
	void OnShow(wxShowEvent& event);
protected:
	bool mOK;
	wxStaticText* mLabel0;
	wxStaticText* mTipText;
	wxTextCtrl* mExpressionText;
	wxRadioButton* mFloatRadio;
	wxRadioButton* mIntRadio;
	wxButton* mButtonOK;
	wxButton* mButtonCancel;
	PropertyWindow* mPropertyWindow;
};

#endif