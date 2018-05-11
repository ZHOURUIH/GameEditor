#include "WindowHeader.h"
#include "txCommandHeader.h"
#include "txComponentAudio.h"

void CommandWindowPlayAudio::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	txComponentAudio* audioComponent = static_cast<txComponentAudio*>(window->getFirstActiveComponent(TOSTRING(txComponentAudio)));
	if (audioComponent != NULL)
	{
		audioComponent->play(mSoundName, mLoop, mVolume);
	}
}

std::string CommandWindowPlayAudio::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, loop : %s", mSoundName.c_str(), txStringUtility::boolToString(mLoop).c_str());
}