#ifndef _SQLITE_SUB_REGION_H_
#define _SQLITE_SUB_REGION_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SubRegionData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_MAP_ID;
	static string COL_POSITION;
	static string COL_SIZE;
public:
	int mID;
	string mName;
	int mMapID;
	Vector2i mPosition;
	Vector2i mSize;
public:
	SubRegionData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mMapID, COL_MAP_ID);
		REGISTE_PARAM(mPosition, COL_POSITION);
		REGISTE_PARAM(mSize, COL_SIZE);
	}
};

class SQLiteSubRegion : public SQLiteTable
{
public:
	SQLiteSubRegion(SQLite* sqlite)
		:SQLiteTable("SceneMapSubRegion", sqlite){}
	bool update(const SubRegionData& data);
};

#endif