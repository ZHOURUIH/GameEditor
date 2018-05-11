#include "txCommandHeader.h"
#include "WindowHeader.h"
#include "WindowComponentRotateSpeed.h"

void CommandWindowRotateSpeed::execute()
{
	Window* window = CMD_CAST<Window*>(mReceiver);
	WindowComponentRotateSpeed* component = static_cast<WindowComponentRotateSpeed*>(window->getFirstComponent(TOSTRING(WindowComponentRotateSpeed)));
	if (component != NULL)
	{
		component->setActive(true);
		component->startRotateSpeed(mStartAngle, mRotateSpeed, mRotateAcceleration);
	}
}

std::string CommandWindowRotateSpeed::showDebugInfo()
{
	COMMAND_DEBUG("start : %s, speed : %s, acceleration : %s", txStringUtility::vector3ToString(mStartAngle).c_str(),
		txStringUtility::vector3ToString(mRotateSpeed).c_str(), txStringUtility::vector3ToString(mRotateAcceleration).c_str());
}