#ifndef _SQLITE_EFFECT_H_
#define _SQLITE_EFFECT_H_

#include "ServerDefine.h"
#include "SQLiteTable.h"

class EffectData : public SQLiteTableData
{
public:
	static std::string COL_LABLE;
	static std::string COL_ID;
	static std::string COL_DESC;
	static std::string COL_RESOURCE;
	std::string mLabel;
	int mID;
	std::string mDesc;
	std::string mResource;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mLabel = reader->getString(table->getCol(COL_LABLE));
		mID = reader->getInt(table->getCol(COL_ID));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_LABLE);
		table->registeColumn(COL_ID);
		table->registeColumn(COL_DESC);
		table->registeColumn(COL_RESOURCE);
	}
};

class SQLiteEffect : public SQLiteTable
{
public:
	SQLiteEffect(SQLite* sqlite)
		:SQLiteTable("Effect", sqlite)
	{
		EffectData::registeColumn(this);
	}
	void query(int weaponID, EffectData& data);
	bool insert(const EffectData& data);
};

#endif