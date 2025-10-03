#pragma once

#include "wxHeader.h"
#include "FrameHeader.h"

class EditorApp : public wxApp
{
public:
	EditorApp();
	virtual ~EditorApp() = default;
	virtual bool OnInit();

	DECLARE_EVENT_TABLE()
	void OnKeyDown(wxKeyEvent& event) { event.Skip(); }
	void OnKeyUp(wxKeyEvent& event) { event.Skip(); }
	void OnMouseWheel(wxMouseEvent& event) { event.Skip(); }
protected:
};

DECLARE_APP(EditorApp)