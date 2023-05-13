#ifndef _TD_ANIMATION_H_
#define _TD_ANIMATION_H_

#include "SQLiteData.h"

class TDAnimation : public SQLiteData
{
public:
	static const char* ID;
	static const char* Description;
	static const char* Loop;
	static const char* AnimationSpeed;
	static const char* AnimationPosition;
public:
	int mID;
	string mDescription;
	bool mLoop;
	float mAnimationSpeed;
	myVector<ushort> mAnimationPosition;
public:
	TDAnimation()
	{
		registeParam(mID, ID);
		registeParam(mDescription, Description);
		registeParam(mLoop, Loop);
		registeParam(mAnimationSpeed, AnimationSpeed);
		registeParam(mAnimationPosition, AnimationPosition);
	}
};

#endif