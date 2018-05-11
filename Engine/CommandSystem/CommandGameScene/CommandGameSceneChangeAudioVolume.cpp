#include "txCommandHeader.h"
#include "GameScene.h"
#include "txComponentHeader.h"

void CommandGameSceneChangeAudioVolume::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	txComponentAudio* audioComponent = static_cast<txComponentAudio*>(gameScene->getFirstActiveComponent(TOSTRING(txComponentAudio)));
	if (audioComponent != NULL)
	{
		audioComponent->setVolume(mVolume);
	}
}

std::string CommandGameSceneChangeAudioVolume::showDebugInfo()
{
	COMMAND_DEBUG("volume : %.2f", mVolume);
}