#include "txCommandHeader.h"
#include "GameScene.h"
#include "txComponentHeader.h"

void CommandGameScenePlayAudio::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	txComponentAudio* audioComponent = static_cast<txComponentAudio*>(gameScene->getFirstActiveComponent(TOSTRING(txComponentAudio)));
	if (audioComponent != NULL)
	{
		audioComponent->play(mSoundFileName, mLoop, mVolume);
	}
}

std::string CommandGameScenePlayAudio::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, Loop : %s, volume : %.2f", mSoundFileName.c_str(), txStringUtility::boolToString(mLoop).c_str(), mVolume);
}