#include "CoreCommandHeader.h"
#include "ECEditor.h"

void CommandEditorChangePosition::execute()
{
	ECEditor* editor = CMD_CAST<ECEditor*>(mReceiver);
	editor->setPosition(mPosition, mRefreshNow);
}

std::string CommandEditorChangePosition::showDebugInfo()
{
	COMMAND_DEBUG("position : %.2f, %.2f, %.2f, refrtesh now : %s", mPosition.x, mPosition.y, mPosition.z, mRefreshNow ? "true" : "false");
}