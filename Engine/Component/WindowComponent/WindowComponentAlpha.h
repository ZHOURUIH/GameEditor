#ifndef _WINDOW_COMPONENT_ALPHA_H_
#define _WINDOW_COMPONENT_ALPHA_H_

#include "txComponentAlpha.h"

class WindowComponentAlpha : public txComponentAlpha
{
public:
	WindowComponentAlpha(const std::string& typeName, const std::string& name)
		:
		txComponentAlpha(typeName, name)
	{}
	virtual ~WindowComponentAlpha(){ destroy(); }
	void destroy(){}
	virtual bool isType(const std::string& type) 
	{
		return txComponentAlpha::isType(type) || type == TOSTRING(WindowComponentAlpha);
	}
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
};

#endif