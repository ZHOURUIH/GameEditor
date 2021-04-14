#ifndef _TD_MINE_REGION_H_
#define _TD_MINE_REGION_H_

#include "SQLiteData.h"

class TDMineRegion : public SQLiteData
{
public:
	static const char* ID;
	static const char* MapID;
	static const char* MineTile;
	static const char* Direction;
public:
	int mID;
	int mMapID;
	Vector2UShort mMineTile;
	byte mDirection;
public:
	TDMineRegion()
	{
		registeParam(mID, ID);
		registeParam(mMapID, MapID);
		registeParam(mMineTile, MineTile);
		registeParam(mDirection, Direction);
	}
};

#endif