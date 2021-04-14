#ifndef _TD_REWARD_H_
#define _TD_REWARD_H_

#include "SQLiteData.h"

class TDReward : public SQLiteData
{
public:
	static const char* ID;
	static const char* Type;
	static const char* Description;
	static const char* ObjectList;
	static const char* CountList;
	static const char* Gold;
	static const char* GoldIngot;
	static const char* Exp;
	static const char* Param0;
	static const char* ParamDesc0;
	static const char* Param1;
	static const char* ParamDesc1;
public:
	int mID;
	byte mType;
	string mDescription;
	myVector<ushort> mObjectList;
	myVector<uint> mCountList;
	uint mGold;
	uint mGoldIngot;
	uint mExp;
	string mParam0;
	string mParamDesc0;
	string mParam1;
	string mParamDesc1;
public:
	TDReward()
	{
		registeParam(mID, ID);
		registeParam(mType, Type);
		registeParam(mDescription, Description);
		registeParam(mObjectList, ObjectList);
		registeParam(mCountList, CountList);
		registeParam(mGold, Gold);
		registeParam(mGoldIngot, GoldIngot);
		registeParam(mExp, Exp);
		registeParam(mParam0, Param0);
		registeParam(mParamDesc0, ParamDesc0);
		registeParam(mParam1, Param1);
		registeParam(mParamDesc1, ParamDesc1);
	}
};

#endif