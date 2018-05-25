#include "WindowProperty.h"
#include "Layout.h"
#include "Window.h"

#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "LayoutHierarchy.h"
#include "EditorCommandHeader.h"
#include "InnerChildHierarchy.h"

void CommandEditorFrameNotifyWindowPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<Window*>& selections = mLayoutEditorCore->getSelections();
	// 如果不是在属性窗口中改变的窗口属性,才刷新属性窗口中的显示
	if (mChanger != mPropertyWindow)
	{
		mPropertyWindow->showPropertyT(selections);
	}
	// 如果是窗口的可见性属性改变了
	if (mPropertyName == txProperty::getPropertyName<WindowPropertyVisible>() && mLayoutEditorCore->getCurLayout() != NULL)
	{
		Window* window = mLayoutEditorCore->getCurLayout()->getWindow(mWindowName);
		// 当前只选中了一个窗口,并且选中的是内部子窗口,则改变内部子窗口视图中的可视图标
		if (selections.size() == 1 && selections[0]->isInnerChild())
		{
			mInnerChildHierarchy->setTreeItemVisibleIcon(window);
		}
		else
		{
			mLayoutHierarchy->setVisibleIcon(window);
		}
	}
}

std::string CommandEditorFrameNotifyWindowPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, old property value : %s, property value : %s",
		mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}