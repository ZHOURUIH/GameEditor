#include "WindowComponentAlpha.h"
#include "WindowHeader.h"

void WindowComponentAlpha::applyKeyFrame(const txVector<float>& value)
{
	Window* window = static_cast<Window*>(mComponentOwner);
	float alpha = mStartAlpha + (mTargetAlpha - mStartAlpha) * value[0];
	window->setAlpha(alpha);
}