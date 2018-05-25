#ifndef _CONNECT_WINDOW_DIALOG_H_
#define _CONNECT_WINDOW_DIALOG_H_

#include "EditorDialog.h"

class ConnectWindowDialog : public EditorDialog
{
public:
	ConnectWindowDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ConnectWindowDialog(){}
	void setup();
	DECLARE_EVENT_TABLE()
	void OnOKButton(wxCommandEvent& event);
	void OnCancelButton(wxCommandEvent& event);
protected:
	wxTextCtrl* mOperateWindowNameText;
	wxTextCtrl* mStartWindowNameText;
	wxTextCtrl* mEndWindowNameText;
	wxTextCtrl* mWindowCountText;
	wxCheckBox* mAdjustPosition;
	wxCheckBox* mOKClose;
	wxButton* mOKButton;
	wxButton* mCancelButton;
};

#endif