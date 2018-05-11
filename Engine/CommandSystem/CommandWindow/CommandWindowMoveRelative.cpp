#include "txTrembling.h"
#include "txTremblingManager.h"
#include "txEngineRoot.h"

#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "txComponentHeader.h"

void CommandWindowMoveRelative::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentMoveRelative* comTrembling = static_cast<WindowComponentMoveRelative*>(window->getFirstComponent(TOSTRING(WindowComponentMoveRelative)));
	if (comTrembling != NULL)
	{
		comTrembling->setDoingCallback(mDoingCallBack, mDoingUserData);
		comTrembling->setDoneCallback(mDoneCallBack, mDoneUserData);
		comTrembling->setActive(true);
		comTrembling->setStartPos(mStartPosition);
		comTrembling->play(mName, mLoop, mOnceLength, mOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandWindowMoveRelative::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, loop : %s, once length : %f, offset : %f, fullOnce : %s, amplitude : %f",
		mName.c_str(), mLoop ? "true" : "false", mOnceLength, mOffset, mFullOnce ? "true" : "false", mAmplitude);
}