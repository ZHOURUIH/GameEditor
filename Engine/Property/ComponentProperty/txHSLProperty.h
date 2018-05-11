#ifndef _TX_HSL_PROPERTY_H_
#define _TX_HSL_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(WindowComponentHSL)

// 起始HSL
class txHSLPropertyStartHSL : public WindowComponentHSLProperty<txVector3Property, VECTOR3>
{
public:
	txHSLPropertyStartHSL();
	virtual void setRealValue(const VECTOR3& value, WindowComponentHSL* component);
	virtual VECTOR3 getRealValue(WindowComponentHSL* component);
};

//---------------------------------------------------------------------------------------------------------
// 目标HSL
class txHSLPropertyTargetHSL : public WindowComponentHSLProperty<txVector3Property, VECTOR3>
{
public:
	txHSLPropertyTargetHSL();
	virtual void setRealValue(const VECTOR3& value, WindowComponentHSL* component);
	virtual VECTOR3 getRealValue(WindowComponentHSL* component);
};

#endif