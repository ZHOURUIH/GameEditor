#include "CoreCommandHeader.h"
#include "ECEditor.h"

void CommandEditorRename::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	bool result = editor->rename(mNewName);
	if (mResult != NULL)
	{
		*mResult = result;
	}
}

std::string CommandEditorRename::showDebugInfo()
{
	COMMAND_DEBUG("new name : %s", mNewName.c_str());
}