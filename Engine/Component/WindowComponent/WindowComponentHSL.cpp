#include "WindowComponentHSL.h"
#include "WindowHeader.h"

void WindowComponentHSL::applyKeyFrame(const txVector<float>& value)
{
	Window* window = static_cast<Window*>(mComponentOwner);
	VECTOR3 hsl = mStartHSL + (mTargetHSL - mStartHSL) * value[0];
	window->setHSLOffset(hsl);
}