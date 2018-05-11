#ifndef _COMMAND_WINDOW_ROTATE_SPEED_H_
#define _COMMAND_WINDOW_ROTATE_SPEED_H_

#include "txCommand.h"

class CommandWindowRotateSpeed : public txCommand
{
public:
	virtual void reset()
	{
		mStartAngle = txMath::VEC3_ZERO;
		mRotateSpeed = txMath::VEC3_ONE;
		mRotateAcceleration = txMath::VEC3_ZERO;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	VECTOR3 mStartAngle;
	VECTOR3 mRotateSpeed;
	VECTOR3 mRotateAcceleration;
};

#endif