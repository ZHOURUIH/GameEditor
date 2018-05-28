#ifndef _EDITOR_DIALOG_H_
#define _EDITOR_DIALOG_H_

#include "txCommandReceiver.h"
#include "txEngineBase.h"

#include "EditorCoreBase.h"

#include "EditorBase.h"

class EditorDialog : public wxDialog, public EditorBase, public EditorCoreBase, public txCommandReceiver
{
public:
public:
	EditorDialog(wxWindow* parent, const wxString& title, const wxPoint& position, const wxSize& size, long style, const wxString& name)
		:
		wxDialog(parent, wxID_ANY, title, position, size, style, name),
		txCommandReceiver(title.ToStdString())
	{}
};

#endif