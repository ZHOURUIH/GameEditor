#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentScale.h"

void CommandWindowScale::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentScale* componentScale = static_cast<WindowComponentScale*>(window->getFirstComponent(TOSTRING(WindowComponentScale)));
	if (componentScale != NULL)
	{
		componentScale->setActive(true);
		componentScale->setDoingCallback(mDoingCallback, mDoingUserData);
		componentScale->setDoneCallback(mDoneCallback, mDoneUserData);
		componentScale->setStartScale(VECTOR3(mStartScale.x, mStartScale.y, 1.0f));
		componentScale->setTargetScale(VECTOR3(mTargetScale.x, mTargetScale.y, 1.0f));
		componentScale->play(mName, mLoop, mOnceLength, mTimeOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandWindowScale::showDebugInfo()
{
	COMMAND_DEBUG("scale time : %.2f, start scale : %.2f, %.2f, target scale : %.2f, %.2f",
		mOnceLength, mStartScale.x, mStartScale.y, mTargetScale.x, mTargetScale.y);
}