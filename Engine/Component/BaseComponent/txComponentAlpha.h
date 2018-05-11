#ifndef _TX_COMPONENT_ALPHA_H_
#define _TX_COMPONENT_ALPHA_H_

#include "txComponentKeyFrame.h"

class txComponentAlpha : public txComponentKeyFrame
{
public:
	txComponentAlpha(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{
		mStartAlpha = 0.0f;
		mTargetAlpha = 1.0f;
		clearCallback();
	}
	virtual ~txComponentAlpha(){ destroy(); }
	virtual void initProperty();
	void destroy(){ clearCallback(); }
	void setStartAlpha(const float& start) { mStartAlpha = start; }
	void setTargetAlpha(const float& target) { mTargetAlpha = target; }
	const float& getStartAlpha() { return mStartAlpha; }
	const float& getTargetAlpha(){ return mTargetAlpha; }
protected:
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentAlpha); }
	virtual bool isType(const std::string& type){ return txComponentKeyFrame::isType(type) || type == TOSTRING(txComponentAlpha); }
	virtual void applyKeyFrame(const txVector<float>& value);
protected:
	float mStartAlpha;
	float mTargetAlpha;
};

#endif