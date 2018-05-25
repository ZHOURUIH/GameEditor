#include "txScene.h"
#include "txUtility.h"

#include "CoreCommandHeader.h"
#include "ECScene.h"

void CommandSceneSaveHeightMap::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	txScene* engineScene = scene->getScene();
	std::string fullPath = txUtility::getMediaPath() + "/" + engineScene->getName() + "/" + engineScene->getName() + ".heightmap";
	engineScene->saveHeightMapFile(fullPath);
}

std::string CommandSceneSaveHeightMap::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}