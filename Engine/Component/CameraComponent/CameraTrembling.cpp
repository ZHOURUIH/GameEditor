#include "CameraTrembling.h"
#include "GameCamera.h"

void CameraTrembling::applyKeyFrame(const txVector<float>& value)
{
	if (value.size() >= 2)
	{
		mCurOffset.x = value[0];
		mCurOffset.y = value[1];
		mCurOffset.z = 0.0f;
	}
}

void CameraTrembling::lateUpdate(float elapsedTime)
{
	if (!txMath::isFloatZero(txMath::getLength(mCurOffset)))
	{
		const VECTOR3& realPos = static_cast<GameCamera*>(mComponentOwner)->getRealPosition();
		static_cast<GameCamera*>(mComponentOwner)->setRealPositon(realPos + mCurOffset);
	}
	txComponent::lateUpdate(elapsedTime);
}