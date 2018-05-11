#include "txComponentHeader.h"
#include "txRotateProperty.h"
#include "txNode.h"

void txComponentRotate::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txRotatePropertyTargetAngle>();
	addProperty<txRotatePropertyStartAngle>();
}

void txComponentRotate::applyKeyFrame(const txVector<float>& value)
{
	VECTOR3 rot = mStartAngle + (mTargetAngle - mStartAngle) * value[0];
	MATRIX3 mat = txMath::eulerAngleToMatrix3(rot);
	// 如果是第一级组件,则影响变换组件
	if (mParent == NULL)
	{
		txComponentTransform* transform = static_cast<txComponentTransform*>(mComponentOwner->getFirstActiveComponentByBaseType(TOSTRING(txComponentTransform)));
		if (transform != NULL)
		{
			transform->setRotation(mat, true);
		}
	}
	// 如果拥有父组件,则只影响父组件
	else
	{
		if (mParent->getNode() != NULL)
		{
			mParent->getNode()->SetRotationMatrix(mat, true);
		}
	}
}