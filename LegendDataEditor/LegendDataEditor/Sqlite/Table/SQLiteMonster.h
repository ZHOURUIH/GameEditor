#ifndef _SQLITE_MONSTER_H_
#define _SQLITE_MONSTER_H_

#include "SQLiteTable.h"

class MonsterData
{
public:
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mPrefab;
	std::string mResource;
};

class SQLiteMonster : public SQLiteTable
{
	std::string COL_LABLE = "MonsterLabel";
	std::string COL_ID = "MonsterID";
	std::string COL_DESC = "Desc";
	std::string COL_PREFAB = "Prefab";
	std::string COL_RESOURCE = "Resource";
public:
	SQLiteMonster(SQLite* sqlite)
		:SQLiteTable("Monster", sqlite)
	{
		registeColumn(COL_LABLE);
		registeColumn(COL_ID);
		registeColumn(COL_DESC);
		registeColumn(COL_PREFAB);
		registeColumn(COL_RESOURCE);
	}
	void query(int monsterID, MonsterData& data);
	bool insert(const MonsterData& data);
protected:
	void parseReader(SQLiteDataReader* reader, MonsterData& data);
};

#endif