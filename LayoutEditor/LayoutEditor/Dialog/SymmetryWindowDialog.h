#ifndef _SYMMETRY_WINDOW_DIALOG_H_
#define _SYMMETRY_WINDOW_DIALOG_H_

#include "EditorDialog.h"

class SymmetryWindowDialog : public EditorDialog
{
public:
	SymmetryWindowDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~SymmetryWindowDialog(){}
	void setup();
	DECLARE_EVENT_TABLE()
	void OnOKButton(wxCommandEvent& event);
	void OnCloseButton(wxCommandEvent& event);
protected:
	wxTextCtrl* mOperateWindowText;
	wxTextCtrl* mSourceWindowText;
	wxComboBox* mXSymmetryModeCombo;
	wxCheckBox* mConsiderWindowSizeXCheck;
	wxComboBox* mYSymmetryModeCombo;
	wxCheckBox* mConsiderWindowSizeYCheck;
	wxButton* mOKButton;
	wxButton* mCloseButton;
};

#endif