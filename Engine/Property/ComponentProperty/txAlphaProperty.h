#ifndef _TX_ALPHA_PROPERTY_H_
#define _TX_ALPHA_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentAlpha);

//---------------------------------------------------------------------------------------------------------
// 起始透明度
class txAlphaPropertyStartAlpha : public txComponentAlphaProperty<txFloatProperty, float>
{
public:
	txAlphaPropertyStartAlpha();
	virtual void setRealValue(const float& value, txComponentAlpha* component);
	virtual float getRealValue(txComponentAlpha* component);
};

//---------------------------------------------------------------------------------------------------------
// 目标透明度
class txAlphaPropertyTargetAlpha : public txComponentAlphaProperty<txFloatProperty, float>
{
public:
	txAlphaPropertyTargetAlpha();
	virtual void setRealValue(const float& value, txComponentAlpha* component);
	virtual float getRealValue(txComponentAlpha* component);
};

#endif