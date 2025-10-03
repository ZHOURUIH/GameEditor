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
	mUndoManager = new UndoManager();
}

bool EditorApp::OnInit()
{
	EditorFrame* mainFrame = new EditorFrame("CSVEditor", wxSize(1440, 900));
	mainFrame->init();
	mainFrame->Show(true);
	return true;
}