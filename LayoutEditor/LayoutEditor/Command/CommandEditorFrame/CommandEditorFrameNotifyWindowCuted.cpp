#include "WindowHeader.h"
#include "Layout.h"

#include "LayoutEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "LayoutHierarchy.h"

void CommandEditorFrameNotifyWindowCuted::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	Window* window = mLayoutEditorCore->getCurLayout()->getWindow(mCutWindowName);
	mLayoutHierarchy->resetParentItem(window);
}

std::string CommandEditorFrameNotifyWindowCuted::showDebugInfo()
{
	COMMAND_DEBUG("window name : %s", mCutWindowName.c_str());
}