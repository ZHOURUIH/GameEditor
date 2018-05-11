#include "txCommandHeader.h"
#include "GameScene.h"
#include "txComponentHeader.h"

void CommandGameSceneStopAudio::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	txComponentAudio* audioComponent = static_cast<txComponentAudio*>(gameScene->getFirstActiveComponent(TOSTRING(txComponentAudio)));
	if (audioComponent != NULL)
	{
		audioComponent->stop();
	}
}

std::string CommandGameSceneStopAudio::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}