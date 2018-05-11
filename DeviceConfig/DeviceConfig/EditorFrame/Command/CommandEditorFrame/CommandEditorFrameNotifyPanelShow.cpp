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
	txMap<wxWindow*, int>& windowIDList = editorFrame->getWindowIDList();
	txMap<wxWindow*, int>::iterator iterWindow = windowIDList.find(window);
	if (iterWindow == windowIDList.end())
	{
		return;
	}
	if (editorFrame->getWindowToolBar() != NULL)
	{
		editorFrame->getWindowToolBar()->ToggleTool(iterWindow->second, mShow);
	}
}

std::string CommandEditorFrameNotifyPanelShow::showDebugInfo()
{
	COMMAND_DEBUG("panel name : %s, show : %s", mPanelName.c_str(), mShow ? "true" : "false");
}