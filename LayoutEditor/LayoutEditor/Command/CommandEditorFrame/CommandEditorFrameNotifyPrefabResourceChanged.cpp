#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "PrefabResource.h"

void CommandEditorFrameNotifyPrefabResourceChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	mPrefabResource->refreshList();
}

std::string CommandEditorFrameNotifyPrefabResourceChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}