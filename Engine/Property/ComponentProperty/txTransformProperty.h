#ifndef _TX_TRANSFORM_PROPERTY_H_
#define _TX_TRANSFORM_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentTransform)

// Î»ÖÃ
class txTransformPropertyPosition : public txComponentTransformProperty<txVector3Property, VECTOR3>
{
public:
	txTransformPropertyPosition();
	virtual void setRealValue(const VECTOR3& value, txComponentTransform* component);
	virtual VECTOR3 getRealValue(txComponentTransform* component);
};

//---------------------------------------------------------------------------------------------------------
// Ðý×ª
class txTransformPropertyRotation : public txComponentTransformProperty<txVector3Property, VECTOR3>
{
public:
	txTransformPropertyRotation();
	virtual void setRealValue(const VECTOR3& value, txComponentTransform* component);
	virtual VECTOR3 getRealValue(txComponentTransform* component);
};

//---------------------------------------------------------------------------------------------------------
// Ëõ·Å
class txTransformPropertyScale : public txComponentTransformProperty<txVector3Property, VECTOR3>
{
public:
	txTransformPropertyScale();
	virtual void setRealValue(const VECTOR3& value, txComponentTransform* component);
	virtual VECTOR3 getRealValue(txComponentTransform* component);
};

#endif