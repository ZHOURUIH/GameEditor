#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutAdded::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	Layout* layout = mLayoutEditorCore->getCurLayout();
	// 刷新layout的层级视图
	mLayoutHierarchy->displayLayout(layout);
	// 如果已加载列表中没有,则加入到已加载列表中
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutAdded::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s", mName.c_str());
}