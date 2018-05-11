#include "WindowComponentScale.h"
#include "WindowHeader.h"

void WindowComponentScale::applyKeyFrame(const txVector<float>& value)
{
	Window* window = static_cast<Window*>(mComponentOwner);
	VECTOR3 scale = mStartScale + (mTargetScale - mStartScale) * value[0];
	window->setMatrixScale(VECTOR2(scale.x, scale.y));
}