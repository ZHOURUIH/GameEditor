#include "WindowComponentMove.h"
#include "WindowHeader.h"

void WindowComponentMove::applyKeyFrame(const txVector<float>& value)
{
	txPoint position = mStartPosition + (mTargetPosition - mStartPosition) * value[0];
	Window* window = static_cast<Window*>(mComponentOwner);
	window->setPosition(position, true);
}