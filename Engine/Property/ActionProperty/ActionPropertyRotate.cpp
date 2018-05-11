#include "ActionPropertyRotate.h"
#include "ActionWindowRotate.h"

ActionWindowRotatePropertyStartRotation::ActionWindowRotatePropertyStartRotation()
{
	mPropertyName = "StartRotation";
	mDescription = "起始旋转值";
}
void ActionWindowRotatePropertyStartRotation::setRealValue(const VECTOR3& value, ActionWindowRotate* action)
{
	action->setStartRotation(value);
}
VECTOR3 ActionWindowRotatePropertyStartRotation::getRealValue(ActionWindowRotate* action)
{
	return action->getStartRotation();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowRotatePropertyTargetRotation::ActionWindowRotatePropertyTargetRotation()
{
	mPropertyName = "TargetRotation";
	mDescription = "终止旋转值";
}
void ActionWindowRotatePropertyTargetRotation::setRealValue(const VECTOR3& value, ActionWindowRotate* action)
{
	action->setTargetRotation(value);
}
VECTOR3 ActionWindowRotatePropertyTargetRotation::getRealValue(ActionWindowRotate* action)
{
	return action->getTargetRotation();
}