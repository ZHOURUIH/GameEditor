#include "Layout.h"

#include "ECLog.h"
#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyWindowPosChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	Layout* layout = mLayoutEditorCore->getCurLayout();
	// 刷新layout的层级视图
	Window* window = layout->getWindow(mName);
	if (window == NULL)
	{
		EDITOR_CORE_ERROR("error : can not find window, name : %s", mName.c_str());
		return;
	}
	mLayoutHierarchy->moveItemPosition(mParentName, window, mOldPosition, mNewPosition);
	txVector<std::string> nameList;
	nameList.push_back(mName);
	mLayoutHierarchy->setSelection(nameList);
}

std::string CommandEditorFrameNotifyWindowPosChanged::showDebugInfo()
{
	COMMAND_DEBUG("window name : %s, parent name : %s, old pos : %d, new pos : %d", mName.c_str(), mParentName.c_str(), mOldPosition, mNewPosition);
}