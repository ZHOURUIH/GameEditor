#include "EditorFrame.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutVisibleChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 设置已加载布局中的对应布局的显示图标
	mLoadedLayout->setItemImage(mLayoutName, mVisible);
}

std::string CommandEditorFrameNotifyLayoutVisibleChanged::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s, visible : %s", mLayoutName.c_str(), mVisible ? "true" : "false");
}