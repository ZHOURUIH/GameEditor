#ifndef _EDITOR_PANEL_H_
#define _EDITOR_PANEL_H_

#include "ToolCoreBase.h"
#include "EditorHeader.h"
#include "EditorBase.h"

class EditorPanel : public wxPanel, public EditorBase, public ToolCoreBase
{
public:
	EditorPanel(wxWindow* parent, wxPoint position, wxSize size, long style, wxString name)
		:
		wxPanel(parent, wxID_ANY, position, size, style, name)
	{}

	DECLARE_EVENT_TABLE()
	void OnShow(wxShowEvent& event);
};

#endif