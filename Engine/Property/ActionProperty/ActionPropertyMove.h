#ifndef _ACTION_PROPERTY_MOVE_H_
#define _ACTION_PROPERTY_MOVE_H_

#include "WindowActionProperty.h"

DEFINE_BASE_PROPERTY(ActionWindowMove);

//---------------------------------------------------------------------------------------------------------
// 起始透明度
class ActionWindowMovePropertyStartPos : public ActionWindowMoveProperty<txPointProperty, txPoint>
{
public:
	ActionWindowMovePropertyStartPos();
	virtual void setRealValue(const txPoint& value, ActionWindowMove* action);
	virtual txPoint getRealValue(ActionWindowMove* action);
};
//---------------------------------------------------------------------------------------------------------
// 终止透明度
class ActionWindowMovePropertyTargetPos : public ActionWindowMoveProperty<txPointProperty, txPoint>
{
public:
	ActionWindowMovePropertyTargetPos();
	virtual void setRealValue(const txPoint& value, ActionWindowMove* action);
	virtual txPoint getRealValue(ActionWindowMove* action);
};

#endif