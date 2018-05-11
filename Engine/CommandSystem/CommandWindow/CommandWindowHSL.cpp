#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentHSL.h"

void CommandWindowHSL::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentHSL* componentHSL = static_cast<WindowComponentHSL*>(window->getFirstComponent(TOSTRING(WindowComponentHSL)));
	if (componentHSL != NULL)
	{
		componentHSL->setActive(true);
		componentHSL->setDoingCallback(mDoingCallback, mDoingUserData);
		componentHSL->setDoneCallback(mDoneCallback, mDoneUserData);
		componentHSL->setStartHSL(mStartHSL);
		componentHSL->setTargetHSL(mTargetHSL);
		componentHSL->play(mName, mLoop, mOnceLength, mTimeOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandWindowHSL::showDebugInfo()
{
	COMMAND_DEBUG("start hsl : %s, target hsl : %s, fade time : %.3f", txStringUtility::vector3ToString(mStartHSL).c_str(),
		txStringUtility::vector3ToString(mTargetHSL).c_str(), mOnceLength);
}