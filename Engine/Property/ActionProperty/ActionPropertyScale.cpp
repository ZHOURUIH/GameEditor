#include "ActionPropertyScale.h"
#include "ActionWindowScale.h"

ActionWindowScalePropertyStartScale::ActionWindowScalePropertyStartScale()
{
	mPropertyName = "StartScale";
	mDescription = "起始缩放值";
}
void ActionWindowScalePropertyStartScale::setRealValue(const VECTOR2& value, ActionWindowScale* action)
{
	action->setStartScale(value);
}
VECTOR2 ActionWindowScalePropertyStartScale::getRealValue(ActionWindowScale* action)
{
	return action->getStartScale();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowScalePropertyTargetScale::ActionWindowScalePropertyTargetScale()
{
	mPropertyName = "TargetScale";
	mDescription = "终止缩放值";
}
void ActionWindowScalePropertyTargetScale::setRealValue(const VECTOR2& value, ActionWindowScale* action)
{
	action->setTargetScale(value);
}
VECTOR2 ActionWindowScalePropertyTargetScale::getRealValue(ActionWindowScale* action)
{
	return action->getTargetScale();
}