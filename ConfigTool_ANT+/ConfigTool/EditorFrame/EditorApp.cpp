#include "EditorApp.h"
#include "EditorFrame.h"

IMPLEMENT_APP(EditorApp)

BEGIN_EVENT_TABLE(EditorApp, wxApp)
EVT_KEY_DOWN(OnKeyDown)
EVT_KEY_UP(OnKeyUp)
EVT_MOUSEWHEEL(OnMouseWheel)

END_EVENT_TABLE()

EditorApp::EditorApp()
:
mMainFrame(NULL)
{
	;
}

EditorApp::~EditorApp()
{
	mMainFrame = NULL;
}

void EditorApp::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnKeyUp(wxKeyEvent& event)
{
	event.Skip();
}

void EditorApp::OnMouseWheel(wxMouseEvent& event)
{
	event.Skip();
}

bool EditorApp::OnInit()
{
	mMainFrame = new EditorFrame("ConfigTool_ANT+", wxSize(1440, 900));
	mMainFrame->init();
	mMainFrame->Show(true);
	mMainFrame->Maximize();
	return true;
}