#include "AllHeader.h"

IMPLEMENT_APP(EditorApp)

BEGIN_EVENT_TABLE(EditorApp, wxApp)
EVT_KEY_DOWN(OnKeyDown)
EVT_KEY_UP(OnKeyUp)
EVT_MOUSEWHEEL(OnMouseWheel)

END_EVENT_TABLE()

EditorApp::EditorApp()
{
	mEditorApp = this;
	mCSVEditor = new CSVEditor();
}

EditorApp::~EditorApp()
{}

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
	EditorFrame* mainFrame = new EditorFrame("CSVEditor", wxSize(1440, 900));
	mainFrame->init();
	mainFrame->Show(true);

	return true;
}