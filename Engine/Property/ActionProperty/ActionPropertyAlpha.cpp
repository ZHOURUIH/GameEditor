#include "ActionPropertyAlpha.h"
#include "ActionWindowAlpha.h"

ActionWindowAlphaPropertyStartAlpha::ActionWindowAlphaPropertyStartAlpha()
{
	mPropertyName = "StartAlpha";
	mDescription = "起始透明度";
}
void ActionWindowAlphaPropertyStartAlpha::setRealValue(const float& value, ActionWindowAlpha* action)
{
	action->setStartAlpha(value);
}
float ActionWindowAlphaPropertyStartAlpha::getRealValue(ActionWindowAlpha* action)
{
	return action->getStartAlpha();
}
//---------------------------------------------------------------------------------------------------------
ActionWindowAlphaPropertyTargetAlpha::ActionWindowAlphaPropertyTargetAlpha()
{
	mPropertyName = "TargetAlpha";
	mDescription = "终止透明度";
}
void ActionWindowAlphaPropertyTargetAlpha::setRealValue(const float& value, ActionWindowAlpha* action)
{
	action->setTargetAlpha(value);
}
float ActionWindowAlphaPropertyTargetAlpha::getRealValue(ActionWindowAlpha* action)
{
	return action->getTargetAlpha();
}