#include "txComponentHeader.h"
#include "txNode.h"

void txComponentMove::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 pos = mStartPosition + (mTargetPosition - mStartPosition) * value[0];
	// 如果是第一级组件,则影响变换组件
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setPosition(pos);
		}
	}
	// 如果拥有父组件,则只影响父组件
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetPosition(pos);
		}
	}
}