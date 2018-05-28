#ifndef _EDITOR_PANEL_H_
#define _EDITOR_PANEL_H_

#include "txCommandReceiver.h"
#include "txEngineBase.h"

#include "EditorCoreBase.h"

#include "EditorHeader.h"
#include "EditorBase.h"

class EditorPanel : public wxPanel, public txCommandReceiver, public EditorBase, public EditorCoreBase
{
public:
	EditorPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
		:
		wxPanel(parent, wxID_ANY, position, size, style, name),
		txCommandReceiver(name.ToStdString())
	{}
	virtual ~EditorPanel(){}

	DECLARE_EVENT_TABLE()
	void OnShow(wxShowEvent& event);
};

#endif