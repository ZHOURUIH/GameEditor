#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorBusyInfo.h"

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

void EditorApp::destroyBusyInfo()
{
	TRACE_DELETE(mBusyInfo);
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
	mBusyInfo = TRACE_NEW(EditorBusyInfo, mBusyInfo);
	mMainFrame = new EditorFrame("DeviceConfig", wxSize(950, 600));
	mMainFrame->init();
	mMainFrame->Show(true);
	return true;
}