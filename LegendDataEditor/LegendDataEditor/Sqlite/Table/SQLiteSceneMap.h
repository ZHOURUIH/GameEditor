#ifndef _SQLITE_SCENE_MAP_H_
#define _SQLITE_SCENE_MAP_H_

#include "SQLiteTable.h"

class SceneMapData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_LABEL;
	static string COL_RESOURCE;
	static string COL_MINI_MAP;
	static string COL_WIDTH;
	static string COL_HEIGHT;
	int mID;
	string mLabel;
	string mResource;
	int mMiniMap;
	int mWidth;
	int mHeight;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mID = reader->getInt(table->getCol(COL_ID));
		mLabel = reader->getString(table->getCol(COL_LABEL));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
		mMiniMap = reader->getInt(table->getCol(COL_MINI_MAP));
		mWidth = reader->getInt(table->getCol(COL_WIDTH));
		mHeight = reader->getInt(table->getCol(COL_HEIGHT));
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ID);
		table->registeColumn(COL_LABEL);
		table->registeColumn(COL_RESOURCE);
		table->registeColumn(COL_MINI_MAP);
		table->registeColumn(COL_WIDTH);
		table->registeColumn(COL_HEIGHT);
	}
};

class SQLiteSceneMap : public SQLiteTable
{
public:
	SQLiteSceneMap(SQLite* sqlite)
		:SQLiteTable("SceneMap", sqlite)
	{
		SceneMapData::registeColumn(this);
	}
	void query(int id, SceneMapData& data);
	void queryAll(txVector<SceneMapData>& dataList);
	bool insert(const SceneMapData& data);
	bool update(const SceneMapData& data);
};

#endif