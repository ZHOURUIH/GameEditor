#ifndef _SQLITE_SCENE_MAP_H_
#define _SQLITE_SCENE_MAP_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SceneMapData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_LABEL;
	static string COL_RESOURCE;
	static string COL_MINI_MAP;
	static string COL_WIDTH;
	static string COL_HEIGHT;
	static string COL_MONSTER_REGION;
	static string COL_NPC;
	static string COL_TRANSFER_POINT;
public:
	int mID;
	string mLabel;
	string mResource;
	int mMiniMap;
	int mWidth;
	int mHeight;
	txVector<int> mMonsterRegion;
	txVector<int> mNPC;
	txVector<int> mTransferPoint;
public:
	SceneMapData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mLabel, COL_LABEL);
		REGISTE_PARAM(mResource, COL_RESOURCE);
		REGISTE_PARAM(mMiniMap, COL_MINI_MAP);
		REGISTE_PARAM(mWidth, COL_WIDTH);
		REGISTE_PARAM(mHeight, COL_HEIGHT);
		REGISTE_PARAM(mMonsterRegion, COL_MONSTER_REGION);
		REGISTE_PARAM(mNPC, COL_NPC);
		REGISTE_PARAM(mTransferPoint, COL_TRANSFER_POINT);
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