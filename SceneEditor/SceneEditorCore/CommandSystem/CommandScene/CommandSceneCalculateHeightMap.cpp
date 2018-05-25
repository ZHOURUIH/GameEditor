#include "CoreCommandHeader.h"
#include "ECScene.h"

void CommandSceneCalculateHeightMap::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	scene->clearIgnoreEntityList();
	scene->calculateSceneHeightMap(1);
}

std::string CommandSceneCalculateHeightMap::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}