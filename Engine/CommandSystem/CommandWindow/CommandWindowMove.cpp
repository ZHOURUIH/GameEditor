#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentMove.h"

void CommandWindowMove::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentMove* comMove = static_cast<WindowComponentMove*>(window->getFirstComponent(TOSTRING(WindowComponentMove)));
	if (comMove != NULL)
	{
		comMove->setActive(true);
		comMove->setDoingCallback(mDoingCallback, mDoingUserData);
		comMove->setDoneCallback(mDoneCallback, mDoneUserData);
		comMove->setStartPosition(mStartPosition);
		comMove->setTargetPosition(mTargetPosition);
		comMove->play(mName, mLoop, mOnceLength, mTimeOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandWindowMove::showDebugInfo()
{
	COMMAND_DEBUG("move time : %.2f, start pos : %s, dest pos : %s", mOnceLength,
		txStringUtility::pointToString(mStartPosition).c_str(), txStringUtility::pointToString(mTargetPosition).c_str());
}