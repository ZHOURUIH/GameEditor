#ifndef _TD_MONSTER_REGION_H_
#define _TD_MONSTER_REGION_H_

#include "SQLiteData.h"

class TDMonsterRegion : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapID;
	static const char* Pos;
	static const char* Direction;
	static const char* MonsterID;
	static const char* MonsterLabel;
	static const char* Radius;
	static const char* Count;
	static const char* RefreshInterval;
public:
	int mID;
	ushort mMapID;
	Vector2UShort mPos;
	byte mDirection;
	ushort mMonsterID;
	ushort mRadius;
	ushort mCount;
	ushort mRefreshInterval;
public:
	TDMonsterRegion()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParam(mPos, Pos);
		registeParam(mDirection, Direction);
		registeParam(mMonsterID, MonsterID);
		registeParamEmpty(MonsterLabel);
		registeParam(mRadius, Radius);
		registeParam(mCount, Count);
		registeParam(mRefreshInterval, RefreshInterval);
	}
};

#endif