#ifndef _SQLITE_EFFECT_H_
#define _SQLITE_EFFECT_H_

#include "ServerDefine.h"
#include "SQLiteTable.h"

class EffectData : public SQLiteTableData
{
public:
	static string COL_LABEL;
	static string COL_ID;
	static string COL_DESC;
	static string COL_RESOURCE;
public:
	string mLabel;
	int mID;
	string mDesc;
	string mResource;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mLabel = reader->getString(table->getCol(COL_LABEL));
		mID = reader->getInt(table->getCol(COL_ID));
		mDesc = reader->getString(table->getCol(COL_DESC));
		mResource = reader->getString(table->getCol(COL_RESOURCE));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueString(valueString, mLabel);
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mDesc);
		StringUtility::appendValueString(valueString, mResource);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateString(updateString, COL_LABEL, mLabel);
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_DESC, mDesc);
		StringUtility::appendUpdateString(updateString, COL_RESOURCE, mResource);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_LABEL);
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