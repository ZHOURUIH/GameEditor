#include "txAlphaProperty.h"

//---------------------------------------------------------------------------------------------------------
txAlphaPropertyStartAlpha::txAlphaPropertyStartAlpha()
{
	mReceiverType = TOSTRING(txComponentAlpha);
	mPropertyName = "StartAlpha";
	mDescription = "起始透明度";
}
void txAlphaPropertyStartAlpha::setRealValue(const float& value, txComponentAlpha* component)
{
	component->setStartAlpha(value);
}
float txAlphaPropertyStartAlpha::getRealValue(txComponentAlpha* component)
{
	return component->getStartAlpha();
}

//---------------------------------------------------------------------------------------------------------
txAlphaPropertyTargetAlpha::txAlphaPropertyTargetAlpha()
{
	mReceiverType = TOSTRING(txComponentAlpha);
	mPropertyName = "TargetAlpha";
	mDescription = "目标透明度";
}
void txAlphaPropertyTargetAlpha::setRealValue(const float& value, txComponentAlpha* component)
{
	component->setTargetAlpha(value);
}
float txAlphaPropertyTargetAlpha::getRealValue(txComponentAlpha* component)
{
	return component->getTargetAlpha();
}