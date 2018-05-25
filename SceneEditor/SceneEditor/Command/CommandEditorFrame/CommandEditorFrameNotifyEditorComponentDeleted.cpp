#include "EditorCoreRoot.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyEditorComponentDeleted::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 从组件视图中删除该组件
	mComponentHierarchy->deleteItem(mComponentName);
	// 清空属性窗口
	mPropertyWindow->showProperty(NULL);
}

std::string CommandEditorFrameNotifyEditorComponentDeleted::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s", mEditorName.c_str(), mComponentName.c_str());
}