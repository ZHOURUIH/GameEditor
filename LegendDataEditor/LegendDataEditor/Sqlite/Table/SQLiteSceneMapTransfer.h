#ifndef _SQLITE_SCENE_MAP_TRANSFER_H_
#define _SQLITE_SCENE_MAP_TRANSFER_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SceneMapTransferData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_SOURCE_MAP;
	static string COL_SOURCE_MAP_NAME;
	static string COL_POSITION;
	static string COL_TARGET_MAP;
	static string COL_TARGET_MAP_NAME;
	static string COL_TARGET_POSITION;
public:
	int mID;
	int mSourceMap;
	string mSourceMapName;
	Vector2i mPosition;
	int mTargetMap;
	int mTargetMapName;
	Vector2i mTargetPosition;
public:
	SceneMapTransferData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mSourceMap, COL_SOURCE_MAP);
		REGISTE_PARAM(mSourceMapName, COL_SOURCE_MAP_NAME);
		REGISTE_PARAM(mPosition, COL_POSITION);
		REGISTE_PARAM(mTargetMap, COL_TARGET_MAP);
		REGISTE_PARAM(mTargetMapName, COL_TARGET_MAP_NAME);
		REGISTE_PARAM(mTargetPosition, COL_TARGET_POSITION);
	}
};

class SQLiteSceneMapTransfer : public SQLiteTable
{
public:
	SQLiteSceneMapTransfer(SQLite* sqlite)
		:SQLiteTable("SceneMapTransfer", sqlite){}
	void query(int id, SceneMapTransferData& data);
	bool insert(const SceneMapTransferData& data);
	bool update(const SceneMapTransferData& data);
};

#endif