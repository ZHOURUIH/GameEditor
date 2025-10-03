#pragma once

#include "wxHeader.h"
#include "FrameHeader.h"

class EditorApp : public wxApp
{
public:
	EditorApp();
	virtual ~EditorApp();
	virtual bool OnInit();

	DECLARE_EVENT_TABLE()
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseWheel(wxMouseEvent& event);
protected:
};

DECLARE_APP(EditorApp)