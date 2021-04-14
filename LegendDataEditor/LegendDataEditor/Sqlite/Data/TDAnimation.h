#ifndef _TD_ANIMATION_H_
#define _TD_ANIMATION_H_

#include "SQLiteData.h"

class TDAnimation : public SQLiteData
{
public:
	static const char* ID;
	static const char* Description;
	static const char* Atlas;
	static const char* Animation;
	static const char* FrameCount;
	static const char* Loop;
	static const char* AnimationSpeed;
	static const char* DirectionCount;
	static const char* AnimationPosition;
public:
	int mID;
	byte mFrameCount;
	float mAnimationSpeed;
public:
	TDAnimation()
	{
		registeParam(mID, ID);
		registeParamEmpty(Description);
		registeParamEmpty(Atlas);
		registeParamEmpty(Animation);
		registeParam(mFrameCount, FrameCount);
		registeParamEmpty(Loop);
		registeParam(mAnimationSpeed, AnimationSpeed);
		registeParamEmpty(DirectionCount);
		registeParamEmpty(AnimationPosition);
	}
};

#endif