#include "txScene.h"

#include "CoreCommandHeader.h"
#include "ECScene.h"

void CommandSceneShowHeightMap::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	scene->getScene()->setShowHeightMap(mShow);
}

std::string CommandSceneShowHeightMap::showDebugInfo()
{
	COMMAND_DEBUG("show : %s", mShow ? "true" : "false");
}