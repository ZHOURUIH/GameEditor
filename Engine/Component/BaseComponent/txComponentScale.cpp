#include "txComponentHeader.h"
#include "txScaleProperty.h"
#include "txNode.h"

void txComponentScale::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txScalePropertyStartScale>();
	addProperty<txScalePropertyTargetScale>();
}

void txComponentScale::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 scale = mStartScale + (mTargetScale - mStartScale) * value[0];
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setScale(scale, true);
		}
	}
	// 如果拥有父组件,则只影响父组件
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetScale(scale, true);
		}
	}
}