#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

void CommandSceneDestroyEditor::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	mSceneEditorCore->clearEditorSelection();
	mComponentEditorCore->clearComponentSelections();
	// 销毁编辑体
	scene->destroyEditor(mEditorName);
	
	// 发送编辑体销毁事件
	mEditorCoreRoot->sendEvent(CE_EDITOR_DELETED, mEditorName);
}

std::string CommandSceneDestroyEditor::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}