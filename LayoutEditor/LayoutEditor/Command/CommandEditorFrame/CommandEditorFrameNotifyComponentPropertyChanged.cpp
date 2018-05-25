#include "PropertyHeader.h"

#include "WindowProperty.h"
#include "Layout.h"
#include "Window.h"

#include "ComponentEditorCore.h"
#include "LayoutEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "EditorCommandHeader.h"
#include "ComponentPropertyPanel.h"
#include "ComponentViewer.h"

void CommandEditorFrameNotifyComponentPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	txVector<txComponent*>& selections = mComponentEditorCore->getComponentSelections();
	// 如果不是在属性窗口中改变的组件属性,才刷新属性窗口中的显示
	if (mChanger != editorFrame->getComponentPropertyPanel())
	{
		mComponentPropertyPanel->showPropertyT(selections);
	}
	// 如果是组件的激活性属性改变了
	if (mPropertyName == txProperty::getPropertyName<txComponentPropertyActive>() && mLayoutEditorCore->getCurLayout() != NULL)
	{
		Window* window = mLayoutEditorCore->getCurLayout()->getWindow(mWindowName);
		mComponentViewer->notifyComponentActived(window->getComponent(mComponentName));
	}
}

std::string CommandEditorFrameNotifyComponentPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, old property value : %s, property value : %s",
		mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}