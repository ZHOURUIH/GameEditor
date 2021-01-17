#ifndef _SQLITE_SCENE_MAP_H_
#define _SQLITE_SCENE_MAP_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SceneMapData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_LABEL;
	static string COL_MAP_FILE;
	static string COL_MONSTER_REGION;
	static string COL_NPC;
	static string COL_TRANSFER_POINT;
	static string COL_PEACE_AREA;
	static string COL_MAIN_CITY;
	static string COL_SUB_REGION;
public:
	int mID;
	string mLabel;
	int mMapFile;
	txVector<int> mMonsterRegion;
	txVector<int> mNPC;
	txVector<int> mTransferPoint;
	txVector<int> mPeaceArea;
	int mMainCity;
	txVector<int> mSubRegion;
public:
	SceneMapData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mLabel, COL_LABEL);
		REGISTE_PARAM(mMapFile, COL_MAP_FILE);
		REGISTE_PARAM(mMonsterRegion, COL_MONSTER_REGION);
		REGISTE_PARAM(mNPC, COL_NPC);
		REGISTE_PARAM(mTransferPoint, COL_TRANSFER_POINT);
		REGISTE_PARAM(mPeaceArea, COL_PEACE_AREA);
		REGISTE_PARAM(mMainCity, COL_MAIN_CITY);
		REGISTE_PARAM(mSubRegion, COL_SUB_REGION);
	}
};

class SQLiteSceneMap : public SQLiteTable
{
public:
	SQLiteSceneMap(SQLite* sqlite)
		:SQLiteTable("SceneMap", sqlite){}
	void query(int id, SceneMapData& data);
	bool insert(const SceneMapData& data);
	bool update(const SceneMapData& data);
};

#endif