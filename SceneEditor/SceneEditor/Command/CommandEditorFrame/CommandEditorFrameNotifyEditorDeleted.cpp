#include "EditorCoreRoot.h"
#include "SceneEditorCore.h"

#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "SceneHierarchy.h"
#include "PropertyWindow.h"

void CommandEditorFrameNotifyEditorDeleted::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	mSceneHierarchy->showScene(mSceneEditorCore->getCurScene());
	// Çå¿ÕÊôÐÔÒ³Ãæ
	mPropertyWindow->showProperty(NULL);
}

std::string CommandEditorFrameNotifyEditorDeleted::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}