#include "txHSLProperty.h"

//------------------------------------------------------------------------------------------------------
txHSLPropertyStartHSL::txHSLPropertyStartHSL()
{
	mPropertyName = "StartHSL";
	mDescription = "起始HSL";
}
void txHSLPropertyStartHSL::setRealValue(const VECTOR3& value, WindowComponentHSL* component)
{
	component->setStartHSL(value);
}
VECTOR3 txHSLPropertyStartHSL::getRealValue(WindowComponentHSL* component)
{
	return component->getStartHSL();
}

//------------------------------------------------------------------------------------------------------
txHSLPropertyTargetHSL::txHSLPropertyTargetHSL()
{
	mPropertyName = "TargetHSL";
	mDescription = "目标HSL";
}
void txHSLPropertyTargetHSL::setRealValue(const VECTOR3& value, WindowComponentHSL* component)
{
	component->setTargetHSL(value);
}
VECTOR3 txHSLPropertyTargetHSL::getRealValue(WindowComponentHSL* component)
{
	return component->getTargetHSL();
}