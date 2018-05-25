#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "SceneHierarchy.h"

void CommandEditorFrameNotifySceneClose::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	// Çå¿Õ³¡¾°ÊÓÍ¼
	mSceneHierarchy->showScene(NULL);
}

std::string CommandEditorFrameNotifySceneClose::showDebugInfo()
{
	COMMAND_DEBUG("scene name : %s", mSceneName.c_str());
}