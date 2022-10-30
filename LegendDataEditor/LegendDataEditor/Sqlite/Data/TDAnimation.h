#ifndef _TD_ANIMATION_H_
#define _TD_ANIMATION_H_

#include "SQLiteData.h"

class TDAnimation : public SQLiteData
{
public:
	static const char* ID;
	static const char* Description;
	static const char* FrameCount;
	static const char* Loop;
	static const char* AnimationSpeed;
	static const char* DirectionCount;
	static const char* AnimationPosition;
public:
	int mID;
	string mDescription;
	byte mFrameCount;
	bool mLoop;
	float mAnimationSpeed;
	byte mDirectionCount;
	myVector<ushort> mAnimationPosition;
public:
	TDAnimation()
	{
		registeParam(mID, ID);
		registeParam(mDescription, Description);
		registeParam(mFrameCount, FrameCount);
		registeParam(mLoop, Loop);
		registeParam(mAnimationSpeed, AnimationSpeed);
		registeParam(mDirectionCount, DirectionCount);
		registeParam(mAnimationPosition, AnimationPosition);
	}
};

#endif