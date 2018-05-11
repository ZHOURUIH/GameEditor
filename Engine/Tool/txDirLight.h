#ifndef _TX_DIR_LIGHT_H_
#define _TX_DIR_LIGHT_H_

#include "txObject.h"

class txDirLight: public txObject
{
public:
	txDirLight()
		:
		txObject(OT_DIRECTIONAL_LIGHT)
	{
		mDirection = -txMath::AXIS_Y;
		mColor = VECTOR3(1.0f, 1.0f, 1.0f);
	}
	virtual ~txDirLight(){}
	virtual void NotifyParentUpdated(){}
	virtual void Update(float fElaspedTime) {}
public:
	VECTOR3 mDirection;
	VECTOR3 mColor;
};

#endif