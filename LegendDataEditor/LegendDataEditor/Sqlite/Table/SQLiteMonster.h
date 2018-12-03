#ifndef _SQLITE_MONSTER_H_
#define _SQLITE_MONSTER_H_

#include "SQLiteTable.h"

class MonsterData : public SQLiteTableData
{
public:
	static string COL_LABEL;
	static string COL_ID;
	static string COL_DESC;
	static string COL_PREFAB;
	static string COL_RESOURCE;
public:
	string mLabel;
	int mID;
	string mDesc;
	string mPrefab;
	string mResource;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mLabel = table->getCol(COL_LABEL);
		mID = reader->getInt(table->getCol(COL_ID));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mPrefab = reader->getString(table->getCol(COL_PREFAB));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueString(valueString, mLabel);
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mDesc);
		StringUtility::appendValueString(valueString, mPrefab);
		StringUtility::appendValueString(valueString, mResource);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateString(updateString, COL_LABEL, mLabel);
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_DESC, mDesc);
		StringUtility::appendUpdateString(updateString, COL_PREFAB, mPrefab);
		StringUtility::appendUpdateString(updateString, COL_RESOURCE, mResource);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_LABEL);
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