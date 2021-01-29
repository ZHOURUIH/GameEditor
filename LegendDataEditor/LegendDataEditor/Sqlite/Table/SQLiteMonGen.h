#ifndef _SQLITE_MON_GEN_H_
#define _SQLITE_MON_GEN_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MonGenData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_MAP_ID;
	static string COL_POS;
	static string COL_MONSTER_ID;
	static string COL_MONSTER_LABEL;
	static string COL_RADIUS;
	static string COL_COUNT;
	static string COL_REFRESH_INTERVAL;
public:
	int mID;
	int mMapID;
	Vector2i mPos;
	int mMonsterID;
	string mMonsterLabel;
	int mRadius;
	int mCount;
	int mRefreshInterval;
public:
	MonGenData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mMapID, COL_MAP_ID);
		REGISTE_PARAM(mPos, COL_POS);
		REGISTE_PARAM(mMonsterID, COL_MONSTER_ID);
		REGISTE_PARAM(mMonsterLabel, COL_MONSTER_LABEL);
		REGISTE_PARAM(mRadius, COL_RADIUS);
		REGISTE_PARAM(mCount, COL_COUNT);
		REGISTE_PARAM(mRefreshInterval, COL_REFRESH_INTERVAL);
	}
};

class SQLiteMonGen : public SQLiteTable
{
public:
	SQLiteMonGen(SQLite* sqlite)
		:SQLiteTable("MonsterRegion", sqlite){}
	bool update(const MonGenData& data);
};

#endif