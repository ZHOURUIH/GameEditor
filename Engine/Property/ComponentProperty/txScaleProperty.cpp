#include "txScaleProperty.h"

//-----------------------------------------------------------------------------
txScalePropertyStartScale::txScalePropertyStartScale()
{
	mPropertyName = "StartScale";
	mDescription = "起始缩放值";
}
void txScalePropertyStartScale::setRealValue(const VECTOR3& value, txComponentScale* component)
{
	component->setStartScale(value);
}
VECTOR3 txScalePropertyStartScale::getRealValue(txComponentScale* component)
{
	return component->getStartScale();
}

//-----------------------------------------------------------------------------
txScalePropertyTargetScale::txScalePropertyTargetScale()
{
	mPropertyName = "TargetScale";
	mDescription = "目标缩放值";
}
void txScalePropertyTargetScale::setRealValue(const VECTOR3& value, txComponentScale* component)
{
	component->setTargetScale(value);
}
VECTOR3 txScalePropertyTargetScale::getRealValue(txComponentScale* component)
{
	return component->getTargetScale();
}