#include "txComponentHeader.h"
#include "WindowHeader.h"

void WindowComponentColour::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 curColour = mSourceColour + (mTargetColour - mSourceColour) * value[0];
	static_cast<Window*>(mComponentOwner)->setColour(curColour);
}