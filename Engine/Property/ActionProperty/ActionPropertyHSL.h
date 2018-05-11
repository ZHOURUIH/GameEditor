#ifndef _ACTION_PROPERTY_HSL_H_
#define _ACTION_PROPERTY_HSL_H_

#include "WindowActionProperty.h"

DEFINE_BASE_PROPERTY(ActionWindowHSL);

//---------------------------------------------------------------------------------------------------------
// 起始透明度
class ActionWindowHSLPropertyStartHSL : public ActionWindowHSLProperty<txVector3Property, VECTOR3>
{
public:
	ActionWindowHSLPropertyStartHSL();
	virtual void setRealValue(const VECTOR3& value, ActionWindowHSL* action);
	virtual VECTOR3 getRealValue(ActionWindowHSL* action);
};
//---------------------------------------------------------------------------------------------------------
// 终止透明度
class ActionWindowHSLPropertyTargetHSL : public ActionWindowHSLProperty<txVector3Property, VECTOR3>
{
public:
	ActionWindowHSLPropertyTargetHSL();
	virtual void setRealValue(const VECTOR3& value, ActionWindowHSL* action);
	virtual VECTOR3 getRealValue(ActionWindowHSL* action);
};

#endif