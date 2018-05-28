#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutRename::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 刷新layout的层级视图
	mLayoutHierarchy->renameItem(mOldName, mNewName);
	// 刷新已加载布局列表
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutRename::showDebugInfo()
{
	COMMAND_DEBUG("old name : %s, new name : %s", mOldName.c_str(), mNewName.c_str());
}