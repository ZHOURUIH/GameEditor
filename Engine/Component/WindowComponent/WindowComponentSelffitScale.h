#ifndef _WINDOW_COMPONENT_SELFFIT_SCALE_H_
#define _WINDOW_COMPONENT_SELFFIT_SCALE_H_

#include "txComponent.h"

class WindowComponentSelffitScale : public txComponent
{
public:
	WindowComponentSelffitScale(const std::string& type, const std::string& name)
		:
		txComponent(type, name)
	{}
	virtual ~WindowComponentSelffitScale(){ destroy(); }
	virtual void setBaseType(){ mBaseType = TOSTRING(WindowComponentSelffitScale); }
	virtual void update(float elapsed);
	void destroy(){}
	virtual bool isType(const std::string& type){ return type == TOSTRING(WindowComponentSelffitScale); }
protected:
};

#endif
