#ifndef _TD_IMAGE_POSITION_ANIMATION_H_
#define _TD_IMAGE_POSITION_ANIMATION_H_

#include "SQLiteData.h"

class TDImagePositionAnimation : public SQLiteData
{
public:
	static const char* ID;
	static const char* Atlas;
	static const char* Animation;
	static const char* FrameCount;
	static const char* PosX;
	static const char* PosY;
public:
	int mID;
	string mAtlas;
	string mAnimation;
	int mFrameCount;
	myVector<int> mPosX;
	myVector<int> mPosY;
public:
	TDImagePositionAnimation()
	{
		registeParam(mID, ID);
		registeParam(mAtlas, Atlas);
		registeParam(mAnimation, Animation);
		registeParam(mFrameCount, FrameCount);
		registeParam(mPosX, PosX);
		registeParam(mPosY, PosY);
	}
};

#endif