#include "PropertyHeader.h"

#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyComponentPropertyChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene == NULL)
	{
		return;
	}
	ECEditor* editor = scene->getEditor(mEditorName);
	if (editor == NULL)
	{
		return;
	}
	// 如果不是在属性窗口中改变的窗口属性,才刷新属性窗口中的显示
	if (mChanger != mPropertyWindow)
	{
		mPropertyWindow->showProperty(editor->getComponent(mComponentName));
	}
	// 如果是组件激活状态改变了
	if (mPropertyName == txProperty::getPropertyName<txComponentPropertyActive>())
	{
		mComponentHierarchy->setActiveIcon(editor->getComponent(mComponentName));
	}
}

std::string CommandEditorFrameNotifyComponentPropertyChanged::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s, property name : %s, old value : %s, new value : %s", 
		mEditorName.c_str(), mComponentName.c_str(), mPropertyName.c_str(), mOldPropertyValue.c_str(), mPropertyValue.c_str());
}