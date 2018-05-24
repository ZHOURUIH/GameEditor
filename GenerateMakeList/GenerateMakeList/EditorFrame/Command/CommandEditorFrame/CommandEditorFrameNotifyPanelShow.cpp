#include "EditorFrame.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyPanelShow::execute()
{
	EditorFrame* editorFrame = static_cast<EditorFrame*>(mReceiver);
	wxWindow* window = editorFrame->getWindow(mPanelName);
	if (window == NULL)
	{
		return;
	}
	const std::map<wxWindow*, int>& windowIDList = editorFrame->getWindowIDList();
	std::map<wxWindow*, int>::const_iterator iterWindow = windowIDList.find(window);
	if (iterWindow == windowIDList.end())
	{
		return;
	}
	editorFrame->getWindowToolBar()->ToggleTool(iterWindow->second, mShow);
}

std::string CommandEditorFrameNotifyPanelShow::showDebugInfo()
{
	COMMAND_DEBUG("panel name : %s, show : %s", mPanelName.c_str(), mShow ? "true" : "false");
}