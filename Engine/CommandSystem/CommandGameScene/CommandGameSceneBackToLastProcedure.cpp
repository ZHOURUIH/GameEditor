#include "txCommandHeader.h"
#include "GameScene.h"

void CommandGameSceneBackToLastProcedure::execute()
{
	GameScene* gameScene = CMD_CAST<GameScene*>(mReceiver);
	gameScene->backToLastProcedure(mIntent);
}

std::string CommandGameSceneBackToLastProcedure::showDebugInfo()
{
	COMMAND_DEBUG("intent : %s", mIntent.c_str());
}