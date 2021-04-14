#ifndef _TD_ITEM_CONSUMABLE_H_
#define _TD_ITEM_CONSUMABLE_H_

#include "SQLiteData.h"

class TDItemConsumable : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* DisplayFormat;
	static const char* StdMode;
	static const char* BuffList;
	static const char* MaxUseCount;
	static const char* AniCount;
public:
	int mID;
	string mName;
	string mDisplayFormat;
	byte mStdMode;
	myVector<ushort> mBuffList;
	ushort mMaxUseCount;
	byte mAniCount;
public:
	TDItemConsumable()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDisplayFormat, DisplayFormat);
		registeParam(mStdMode, StdMode);
		registeParam(mBuffList, BuffList);
		registeParam(mMaxUseCount, MaxUseCount);
		registeParam(mAniCount, AniCount);
	}
};

#endif