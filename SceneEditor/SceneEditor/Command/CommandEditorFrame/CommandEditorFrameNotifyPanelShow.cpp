#include "EditorFrame.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyPanelShow::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	wxWindow* window = editorFrame->getWindow(mPanelName);
	if (window == NULL)
	{
		return;
	}
	auto& windowIDList = editorFrame->getWindowIDList();
	auto iterWindow = windowIDList.find(window);
	if (iterWindow == windowIDList.end())
	{
		return;
	}
	editorFrame->getWindowMenu()->Check(iterWindow->second.first, mShow);
	editorFrame->getWindowToolBar()->ToggleTool(iterWindow->second.second, mShow);
}

std::string CommandEditorFrameNotifyPanelShow::showDebugInfo()
{
	COMMAND_DEBUG("panel name : %s, show : %s", mPanelName.c_str(), mShow ? "true" : "false");
}