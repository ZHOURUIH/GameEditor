#ifndef _WINDOW_COMPONENT_ROTATE_H_
#define _WINDOW_COMPONENT_ROTATE_H_

#include "txComponentRotate.h"

class WindowComponentRotate : public txComponentRotate
{
public:
	WindowComponentRotate(const std::string& type, const std::string& name)
		:
		txComponentRotate(type, name)
	{}
	virtual ~WindowComponentRotate(){ destroy(); }
	void destroy(){}
	virtual bool isType(const std::string& type)
	{
		return txComponentRotate::isType(type) || type == TOSTRING(WindowComponentRotate);
	}
protected:
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
};

#endif