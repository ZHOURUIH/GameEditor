#include "CoreCommandHeader.h"
#include "ECScene.h"

void CommandSceneShow::execute()
{
	ECScene* scene = CMD_CAST<ECScene*>(mReceiver);
	scene->setSceneVisible(mShow);
}

std::string CommandSceneShow::showDebugInfo()
{
	COMMAND_DEBUG("show : %s", mShow ? "true" : "false");
}