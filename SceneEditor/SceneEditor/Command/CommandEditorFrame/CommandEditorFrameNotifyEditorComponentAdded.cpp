#include "EditorCoreRoot.h"
#include "ECScene.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "ComponentHierarchy.h"

void CommandEditorFrameNotifyEditorComponentAdded::execute()
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
	// 从组件视图中删除该组件
	mComponentHierarchy->addItem(editor->getComponent(mComponentName), mComponentPos);
	// 清空属性窗口
	mPropertyWindow->showProperty(NULL);
}

std::string CommandEditorFrameNotifyEditorComponentAdded::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s, component name : %s, component pos : %d", mEditorName.c_str(), mComponentName.c_str(), mComponentPos);
}