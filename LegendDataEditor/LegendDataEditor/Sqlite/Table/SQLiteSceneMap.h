#ifndef _SQLITE_SCENE_MAP_H_
#define _SQLITE_SCENE_MAP_H_

#include "SQLiteTable.h"

class SceneMapData : public SQLiteTableData
{
public:
	static std::string COL_ID;
	static std::string COL_LABEL;
	static std::string COL_RESOURCE;
	static std::string COL_MINI_MAP;
	int mID;
	std::string mLabel;
	std::string mResource;
	int mMiniMap;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mID = reader->getInt(table->getCol(COL_ID));
		mLabel = reader->getString(table->getCol(COL_LABEL));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
		mMiniMap = reader->getInt(table->getCol(COL_MINI_MAP));
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ID);
		table->registeColumn(COL_LABEL);
		table->registeColumn(COL_RESOURCE);
		table->registeColumn(COL_MINI_MAP);
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