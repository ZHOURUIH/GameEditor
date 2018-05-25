#include "EditorFrame.h"
#include "EditorCommandHeader.h"

void CommandEditorFrameNotifyUndoEnableChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	editorFrame->enableUndo(mEnabled);
}

std::string CommandEditorFrameNotifyUndoEnableChanged::showDebugInfo()
{
	COMMAND_DEBUG("enable : %s", mEnabled ? "true" : "false");
}