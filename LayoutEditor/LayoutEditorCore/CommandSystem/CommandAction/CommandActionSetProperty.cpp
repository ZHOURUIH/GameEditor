#include "txProperty.h"

#include "WindowHeader.h"
#include "ActionWindow.h"

#include "CommandHeader.h"
#include "ECUndoChangeWindowProperty.h"
#include "ECUndoManager.h"

void CommandActionSetProperty::execute()
{
	ActionWindow* action = CMD_CAST<ActionWindow*>(mReceiver);
	txProperty* property = action->getProperty(mPropertyName);
	if (property == NULL)
	{
		return;
	}
	std::string oldValue = property->get(action);
	action->setProperty(mPropertyName, mPropertyValue);
}

std::string CommandActionSetProperty::showDebugInfo()
{
	COMMAND_DEBUG("property name : %s, property value : %s, operator : 0x%p", mPropertyName.c_str(), mPropertyValue.c_str(), mOperator);
}