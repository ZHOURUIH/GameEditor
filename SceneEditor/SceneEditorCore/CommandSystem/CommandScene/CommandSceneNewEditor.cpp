#include "txScene.h"

#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "CoreCommandHeader.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"
#include "SceneEditorCore.h"

void CommandSceneNewEditor::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	ECEditor* editor = scene->createEditor(mEditorName);
	// 发送创建编辑体的事件
	mEditorCoreRoot->sendEvent(CE_EDITOR_ADDED, editor->getName());
	// 选中
	mSceneEditorCore->setEditorSelection(editor);
	// 刷新场景区域大小
	scene->setRectDirty(true);
}

std::string CommandSceneNewEditor::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}