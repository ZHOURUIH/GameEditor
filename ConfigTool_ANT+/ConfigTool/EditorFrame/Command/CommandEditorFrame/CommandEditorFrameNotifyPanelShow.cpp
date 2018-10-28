#include "EditorFrame.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyPanelShow::execute()
{
	EditorFrame* editorFrame = static_cast<EditorFrame*>(mReceiver);
	int windowID = editorFrame->getPanelID(mPanelName);
	if (windowID < 0)
	{
		return;
	}
	editorFrame->getWindowToolBar()->ToggleTool(windowID, mShow);
}

std::string CommandEditorFrameNotifyPanelShow::showDebugInfo()
{
	COMMAND_DEBUG("panel name : %s, show : %s", mPanelName.c_str(), mShow ? "true" : "false");
}