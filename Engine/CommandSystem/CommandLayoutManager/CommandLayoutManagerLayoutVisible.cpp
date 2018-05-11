#include "txCommandHeader.h"
#include "LayoutManager.h"
#include "Layout.h"

void CommandLayoutManagerLayoutVisible::execute()
{
	LayoutManager* layoutManager = CMD_CAST<LayoutManager*>(mReceiver);
	Layout* layout = NULL;
	if (mLayoutID != -1)
	{
		layout = layoutManager->getLayout(mLayoutID);
	}
	else if (mLayoutName != EMPTY_STRING)
	{
		layout = layoutManager->getLayout(mLayoutName);
	}
	if (layout != NULL)
	{
		if (!mForce)
		{
			layout->setVisible(mVisibility, mImmediately, mParam);
		}
		else
		{
			layout->setVisibleForce(mVisibility);
		}
	}
}

std::string CommandLayoutManagerLayoutVisible::showDebugInfo()
{
	COMMAND_DEBUG("layout id : %d, layout name : %s, visible : %s", mLayoutID, mLayoutName.c_str(), mVisibility ? "true" : "false");
}