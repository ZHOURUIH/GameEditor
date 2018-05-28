#include "EditorCommandHeader.h"
#include "EditorFrame.h"
#include "WindowTemplate.h"

void CommandEditorFrameNotifyTemplateResourceChanged::execute()
{
	EditorFrame* editorFrame = CMD_CAST<EditorFrame*>(mReceiver);
	mWindowTemplate->refreshList();
}

std::string CommandEditorFrameNotifyTemplateResourceChanged::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}