#ifndef _SQLITE_SCENE_MAP_FILE_H_
#define _SQLITE_SCENE_MAP_FILE_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SceneMapFileData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_FILE_NAME;
	static string COL_MINI_MAP;
	static string COL_WIDTH;
	static string COL_HEIGHT;
public:
	int mID;
	string mFileName;
	int mMiniMap;
	int mWidth;
	int mHeight;
public:
	SceneMapFileData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mFileName, COL_FILE_NAME);
		REGISTE_PARAM(mMiniMap, COL_MINI_MAP);
		REGISTE_PARAM(mWidth, COL_WIDTH);
		REGISTE_PARAM(mHeight, COL_HEIGHT);
	}
};

class SQLiteSceneMapFile : public SQLiteTable
{
public:
	SQLiteSceneMapFile(SQLite* sqlite)
		:SQLiteTable("SceneMapFile", sqlite){}
	void query(int id, SceneMapFileData& data);
	bool insert(const SceneMapFileData& data);
	bool update(const SceneMapFileData& data);
};

#endif