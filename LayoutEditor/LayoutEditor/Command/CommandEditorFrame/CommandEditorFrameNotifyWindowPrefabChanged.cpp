#include "Layout.h"
#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyWindowPrefabChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	if (mLayoutEditorCore->getCurLayout() != NULL)
	{
		mLayoutHierarchy->refreshChildItem(mLayoutEditorCore->getCurLayout()->getWindow(mWindowName));
	}
}

std::string CommandEditorFrameNotifyWindowPrefabChanged::showDebugInfo()
{
	COMMAND_DEBUG("window name : %s, prefab name : %s", mWindowName.c_str(), mPrefabName.c_str());
}