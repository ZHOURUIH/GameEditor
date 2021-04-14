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
	myVector<ushort> mBuffList;
	ushort mMaxUseCount;
public:
	TDItemConsumable()
	{
		registeParam(mID, ID);
		registeParamEmpty(Name);
		registeParamEmpty(DisplayFormat);
		registeParamEmpty(StdMode);
		registeParam(mBuffList, BuffList);
		registeParam(mMaxUseCount, MaxUseCount);
		registeParamEmpty(AniCount);
	}
};

#endif