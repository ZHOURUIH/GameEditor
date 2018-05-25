#include "txComponent.h"
#include "CoreCommandHeader.h"
#include "ECEditor.h"

void CommandEditorRenameComponent::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	txComponent* component = editor->getComponent(mComponentName);
	bool result = component->rename(mNewName);
	if (mResult != NULL)
	{
		*mResult = result;
	}
}

std::string CommandEditorRenameComponent::showDebugInfo()
{
	COMMAND_DEBUG("component name : %s, new name : %s", mComponentName.c_str(), mNewName.c_str());
}