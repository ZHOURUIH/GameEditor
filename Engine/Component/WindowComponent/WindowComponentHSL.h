#ifndef _WINDOW_COMPONENT_HSL_H_
#define _WINDOW_COMPONENT_HSL_H_

#include "txComponentKeyFrame.h"

class WindowComponentHSL : public txComponentKeyFrame
{
public:
	WindowComponentHSL(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~WindowComponentHSL(){ destroy(); }
	void destroy(){}
	void setStartHSL(const VECTOR3& startHSL){ mStartHSL = startHSL; }
	void setTargetHSL(const VECTOR3& targetHSL){ mTargetHSL = targetHSL; }
	const VECTOR3& getStartHSL() { return mStartHSL; }
	const VECTOR3& getTargetHSL(){ return mTargetHSL; }
	virtual bool isType(const std::string& type)
	{
		return txComponentKeyFrame::isType(type) || type == TOSTRING(WindowComponentHSL);
	}
protected:
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	VECTOR3 mStartHSL;
	VECTOR3 mTargetHSL;
};

#endif