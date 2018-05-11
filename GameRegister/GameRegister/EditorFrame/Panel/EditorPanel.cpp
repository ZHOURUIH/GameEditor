#include "EditorPanel.h"
#include "EditorCommandHeader.h"
#include "EditorFrame.h"

BEGIN_EVENT_TABLE(EditorPanel, wxPanel)
EVT_SHOW(EditorPanel::OnShow)
END_EVENT_TABLE()

void EditorPanel::OnShow(wxShowEvent& event)
{
	CommandEditorFrameNotifyPanelShow cmd(COMMAND_PARAM);
	cmd.mPanelName = GetName().ToStdString();
	cmd.mShow = event.IsShown();
	mCommandSystem->pushCommand(&cmd, mEditorFrame);
}