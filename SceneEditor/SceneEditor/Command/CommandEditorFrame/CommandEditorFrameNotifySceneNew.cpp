#include "EditorCoreRoot.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "SceneHierarchy.h"

void CommandEditorFrameNotifySceneNew::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// ÏÔÊ¾³¡¾°ÊÓÍ¼
	mSceneHierarchy->showScene(mSceneEditorCore->getCurScene());
}

std::string CommandEditorFrameNotifySceneNew::showDebugInfo()
{
	COMMAND_DEBUG("scene name : %s", mSceneName.c_str());
}