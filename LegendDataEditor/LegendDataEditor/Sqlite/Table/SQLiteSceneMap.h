#ifndef _SQLITE_SCENE_MAP_H_
#define _SQLITE_SCENE_MAP_H_

#include "SQLiteTable.h"

class SceneMapData
{
public:
	int mID;
	std::string mLabel;
	std::string mResource;
};

class SQLiteSceneMap : public SQLiteTable
{
	std::string COL_ID = "ID";
	std::string COL_LABEL = "Label";
	std::string COL_RESOURCE = "Resource";
public:
	SQLiteSceneMap(SQLite* sqlite)
		:SQLiteTable("SceneMap", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_LABEL);
		registeColumn(COL_RESOURCE);
	}
	void query(int id, SceneMapData& data);
	void queryAll(txVector<SceneMapData>& dataList);
	bool insert(const SceneMapData& data);
	bool update(const SceneMapData& data);
protected:
	void parseReader(SQLiteDataReader* reader, SceneMapData& data);
	void parseReader(SQLiteDataReader* reader, txVector<SceneMapData>& dataList);
};

#endif