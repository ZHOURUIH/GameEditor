#include "EditorFrame.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyRedoEnableChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	editorFrame->enableRedo(mEnabled);
}

std::string CommandEditorFrameNotifyRedoEnableChanged::showDebugInfo()
{
	COMMAND_DEBUG("enable : %s", mEnabled ? "true" : "false");
}