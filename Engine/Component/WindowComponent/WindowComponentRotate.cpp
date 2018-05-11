#include "WindowComponentRotate.h"
#include "WindowHeader.h"

void WindowComponentRotate::applyKeyFrame(const txVector<float>& value)
{
	Window* window = static_cast<Window*>(mComponentOwner);
	VECTOR3 rot = mStartAngle + (mTargetAngle - mStartAngle) * value[0];
	window->setMatrixRotation(txMath::eulerAngleToMatrix3(rot), true);
}