#ifndef _WINDOW_COMPONENT_ROTATE_SPEED_H_
#define _WINDOW_COMPONENT_ROTATE_SPEED_H_

#include "txComponentRotateSpeed.h"

class WindowComponentRotateSpeed : public txComponentRotateSpeed
{
public:
	WindowComponentRotateSpeed(const std::string& type, const std::string& name)
		:
		txComponentRotateSpeed(type, name)
	{}
	virtual ~WindowComponentRotateSpeed(){ destroy(); }
	void destroy(){}
	virtual bool isType(const std::string& type)
	{
		return txComponentRotateSpeed::isType(type) || type == TOSTRING(WindowComponentRotateSpeed);
	}
protected:
	virtual void applyRotation(const MATRIX3& rotation);
protected:
};

#endif