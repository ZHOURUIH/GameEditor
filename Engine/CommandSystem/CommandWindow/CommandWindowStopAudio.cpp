#include "WindowHeader.h"
#include "txCommandHeader.h"
#include "txComponentAudio.h"

void CommandWindowStopAudio::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	txComponentAudio* audioComponent = static_cast<txComponentAudio*>(window->getFirstActiveComponent(TOSTRING(txComponentAudio)));
	if (audioComponent != NULL)
	{
		audioComponent->stop();
	}
}

std::string CommandWindowStopAudio::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}