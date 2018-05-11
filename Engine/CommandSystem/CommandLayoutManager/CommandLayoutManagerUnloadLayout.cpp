#include "txCommandHeader.h"
#include "LayoutManager.h"
#include "Layout.h"

void CommandLayoutManagerUnloadLayout::execute()
{
	LayoutManager* layoutManager = CMD_CAST<LayoutManager*>(mReceiver);
	if (mName != EMPTY_STRING)
	{
		layoutManager->destroyLayout(mName);
	}
	else if (mLayoutID != -1)
	{
		layoutManager->destroyLayout(mLayoutID);
	}
}

std::string CommandLayoutManagerUnloadLayout::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, layout id : %d", mName.c_str(), mLayoutID);
}