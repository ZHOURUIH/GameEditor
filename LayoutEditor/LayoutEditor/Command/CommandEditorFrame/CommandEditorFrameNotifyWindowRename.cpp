#include "Layout.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyWindowRename::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 刷新layout的层级视图
	mLayoutHierarchy->renameItem(mOldName, mNewName);
}

std::string CommandEditorFrameNotifyWindowRename::showDebugInfo()
{
	COMMAND_DEBUG("old name : %s, new name : %s", mOldName.c_str(), mNewName.c_str());
}