#ifndef _TD_EFFECT_H_
#define _TD_EFFECT_H_

#include "SQLiteData.h"

class TDEffect : public SQLiteData
{
public:
	static const char* ID;
	static const char* PrefabNoDir;
	static const char* DirectionCount;
	static const char* FrameCount;
	static const char* Speed;
	static const char* Loop;
	static const char* DisplayTop;
	static const char* ImageAnimation;
public:
	int mID;
	string mPrefabNoDir;
	byte mDirectionCount;
	byte mFrameCount;
	float mSpeed;
	bool mLoop;
	bool mDisplayTop;
	myVector<ushort> mImageAnimation;
public:
	TDEffect()
	{
		registeParam(mID, ID);
		registeParam(mPrefabNoDir, PrefabNoDir);
		registeParam(mDirectionCount, DirectionCount);
		registeParam(mFrameCount, FrameCount);
		registeParam(mSpeed, Speed);
		registeParam(mLoop, Loop);
		registeParam(mDisplayTop, DisplayTop);
		registeParam(mImageAnimation, ImageAnimation);
	}
};

#endif