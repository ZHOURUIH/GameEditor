#ifndef _TX_COMPONENT_SCALE_H_
#define _TX_COMPONENT_SCALE_H_

#include "txComponentKeyFrame.h"

class txComponentScale : public txComponentKeyFrame
{
public:
	txComponentScale(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name),
		mStartScale(txMath::VEC3_ONE),
		mTargetScale(txMath::VEC3_ONE)
	{}
	virtual ~txComponentScale(){ destroy(); }
	virtual void initProperty();
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentScale); }
	virtual bool isType(const std::string& type){ return txComponentKeyFrame::isType(type) || type == TOSTRING(txComponentScale); }
	void destroy(){}
	void setStartScale(const VECTOR3& startScale) { mStartScale = startScale; }
	void setTargetScale(const VECTOR3& targetScale) { mTargetScale = targetScale; }
	const VECTOR3& getStartScale() { return mStartScale; }
	const VECTOR3& getTargetScale() { return mTargetScale; }
protected:
	virtual void applyKeyFrame(const txVector<float>& scale);
protected:
	VECTOR3 mStartScale;	// 起始缩放值
	VECTOR3 mTargetScale;	// 目标缩放值
};

#endif