#include "WindowHeader.h"
#include "txCommandHeader.h"

void CommandWindowActive::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	window->setVisible(mActive);
}

std::string CommandWindowActive::showDebugInfo()
{
	COMMAND_DEBUG("visible : %s", txStringUtility::boolToString(mActive).c_str());
}