#include "CoreCommandHeader.h"
#include "ECEditorManager.h"

void CommandEditorManagerCreateEditor::execute()
{
	ECEditorManager* manager = CMD_CAST<ECEditorManager*>(mReceiver);
	manager->createEditor(mName);
}

std::string CommandEditorManagerCreateEditor::showDebugInfo()
{
	COMMAND_DEBUG("name : %s", mName.c_str());
}