#ifndef _TD_ACHIVEMENT_H_
#define _TD_ACHIVEMENT_H_

#include "SQLiteData.h"

class TDAchivement : public SQLiteData
{
public:
	static const char* ID;
	static const char* Name;
	static const char* Description;
	static const char* DetailDescription;
	static const char* Category;
	static const char* Type;
	static const char* TargetValue;
	static const char* Reward;
public:
	int mID;
	string mName;
	string mDescription;
	string mDetailDescription;
	byte mCategory;
	ushort mType;
	int mTargetValue;
	int mReward;
public:
	TDAchivement()
	{
		registeParam(mID, ID);
		registeParam(mName, Name);
		registeParam(mDescription, Description);
		registeParam(mDetailDescription, DetailDescription);
		registeParam(mCategory, Category);
		registeParam(mType, Type);
		registeParam(mTargetValue, TargetValue);
		registeParam(mReward, Reward);
	}
};

#endif