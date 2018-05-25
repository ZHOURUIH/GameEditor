#include "Layout.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyWindowAdded::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	::Layout* layout = mLayoutEditorCore->getCurLayout();
	// 刷新layout的层级视图
	mLayoutHierarchy->addItem(layout->getWindow(mName), mWindowPos);
}

std::string CommandEditorFrameNotifyWindowAdded::showDebugInfo()
{
	COMMAND_DEBUG("window name : %s, parent name : %s, window pos : %d", mName.c_str(), mParentName.c_str(), mWindowPos);
}