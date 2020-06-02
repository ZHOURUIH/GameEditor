#ifndef _SQLITE_PEACE_AREA_H_
#define _SQLITE_PEACE_AREA_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class PeaceAreaData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_MAP_ID;
	static string COL_MAP_NAME;
	static string COL_POSITION;
	static string COL_SIZE;
public:
	int mID;
	int mMapID;
	string mMapName;
	Vector2i mPosition;
	Vector2i mSize;
public:
	PeaceAreaData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mMapID, COL_MAP_ID);
		REGISTE_PARAM(mMapName, COL_MAP_NAME);
		REGISTE_PARAM(mPosition, COL_POSITION);
		REGISTE_PARAM(mSize, COL_SIZE);
	}
};

class SQLitePeaceArea : public SQLiteTable
{
public:
	SQLitePeaceArea(SQLite* sqlite)
		:SQLiteTable("SceneMapPeaceArea", sqlite){}
	bool update(const PeaceAreaData& data);
};

#endif