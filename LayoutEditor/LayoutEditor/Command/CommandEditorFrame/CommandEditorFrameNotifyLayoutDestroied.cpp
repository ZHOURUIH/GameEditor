#include "Layout.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "LayoutHierarchy.h"
#include "PropertyWindow.h"
#include "LoadedLayout.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyLayoutDestroied::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// 如果销毁的是当前正在编辑的布局(为空则表示当前布局已经被销毁了)
	if (mLayoutEditorCore->getCurLayout() == NULL || mLayoutName == mLayoutEditorCore->getCurLayout()->getName())
	{
		// 刷新layout的层级视图
		mLayoutHierarchy->displayLayout(NULL);
		// 清空属性窗口
		mPropertyWindow->clearProperty();
	}
	// 刷新已加载布局列表
	mLoadedLayout->refreshList();
}

std::string CommandEditorFrameNotifyLayoutDestroied::showDebugInfo()
{
	COMMAND_DEBUG("layout name : %s", mLayoutName.c_str());
}