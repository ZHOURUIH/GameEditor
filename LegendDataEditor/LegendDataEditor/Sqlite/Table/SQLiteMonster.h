#ifndef _SQLITE_MONSTER_H_
#define _SQLITE_MONSTER_H_

#include "SQLiteTable.h"

class MonsterData : public SQLiteTableData
{
public:
	static string COL_LABLE;
	static string COL_ID;
	static string COL_DESC;
	static string COL_PREFAB;
	static string COL_RESOURCE;
	string mLabel;
	int mID;
	string mDesc;
	string mPrefab;
	string mResource;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mLabel = reader->getString(table->getCol(COL_LABLE));
		mID = reader->getInt(table->getCol(COL_ID));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mPrefab = reader->getString(table->getCol(COL_PREFAB));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_LABLE);
		table->registeColumn(COL_ID);
		table->registeColumn(COL_DESC);
		table->registeColumn(COL_PREFAB);
		table->registeColumn(COL_RESOURCE);
	}
};

class SQLiteMonster : public SQLiteTable
{
public:
	SQLiteMonster(SQLite* sqlite)
		:SQLiteTable("Monster", sqlite)
	{
		MonsterData::registeColumn(this);
	}
	void query(int monsterID, MonsterData& data);
	bool insert(const MonsterData& data);
};

#endif