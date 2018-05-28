#include "CoreCommandHeader.h"
#include "ECEditorManager.h"

void CommandEditorManagerDestroyEditor::execute()
{
	ECEditorManager* editorManager = CMD_CAST<ECEditorManager*>(mReceiver);
	if (mEditor != NULL)
	{
		editorManager->destroyEditor(mEditor);
	}
	else
	{
		editorManager->destroyEditor(mEditorName);
	}
}

std::string CommandEditorManagerDestroyEditor::showDebugInfo()
{
	COMMAND_DEBUG("editor name : %s", mEditorName.c_str());
}