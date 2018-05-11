#ifndef _WINDOW_COMPONENT_SCALE_H_
#define _WINDOW_COMPONENT_SCALE_H_

#include "txComponentScale.h"

class WindowComponentScale : public txComponentScale
{
public:
	WindowComponentScale(const std::string& type, const std::string& name)
		:
		txComponentScale(type, name)
	{}
	virtual ~WindowComponentScale(){ destroy(); }
	void destroy(){}
	virtual bool isType(const std::string& type)
	{
		return txComponentScale::isType(type) || type == TOSTRING(WindowComponentScale);
	}
protected:
	virtual void applyKeyFrame(const txVector<float>& scale);
};

#endif