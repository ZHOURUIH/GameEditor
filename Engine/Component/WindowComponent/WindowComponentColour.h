#ifndef _WINDOW_COMPONENT_COLOUR_H_
#define _WINDOW_COMPONENT_COLOUR_H_

#include "txComponentKeyFrame.h"

class WindowComponentColour : public txComponentKeyFrame
{
public:
	WindowComponentColour(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~WindowComponentColour(){ destroy(); }
	void destroy(){}
	virtual bool isType(const std::string& type){return type == TOSTRING(WindowComponentColour);}
	void setSourceColour(const VECTOR3& colour) { mSourceColour = colour; }
	void setTargetColour(const VECTOR3& colour) { mTargetColour = colour; }
	const VECTOR3& getSourceColour() { return mSourceColour; }
	const VECTOR3& getTargetColour() { return mTargetColour; }
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	VECTOR3 mSourceColour;
	VECTOR3 mTargetColour;
};

#endif