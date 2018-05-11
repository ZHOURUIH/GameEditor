#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentRotate.h"

void CommandWindowRotate::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentRotate* component = static_cast<WindowComponentRotate*>(window->getFirstComponent(TOSTRING(WindowComponentRotate)));
	if (component != NULL)
	{
		component->setDoneCallback(mDoneCallback, mDoneUserData);
		component->setDoingCallback(mDoingCallback, mDoingUserData);
		component->setActive(true);
		component->setStartAngle(mStartAngle);
		component->setTargetAngle(mTargetAngle);
		component->play(mName, mLoop, mOnceLength, mTimeOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandWindowRotate::showDebugInfo()
{
	COMMAND_DEBUG("start : %s, target : %s, rotate time : %.3f, time offset : %.3f", txStringUtility::vector3ToString(mStartAngle).c_str(),
		txStringUtility::vector3ToString(mTargetAngle).c_str(), mOnceLength, mTimeOffset);
}