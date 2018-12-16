#ifndef _SQLITE_MON_GEN_H_
#define _SQLITE_MON_GEN_H_

#include "SQLiteTable.h"

class MonGenData : public SQLiteTableData
{
public:
	static string COL_MAP;
	static string COL_POSX;
	static string COL_POSY;
	static string COL_ID;
	static string COL_MONSTER_LABEL;
	static string COL_RADIUS;
	static string COL_COUNT;
	static string COL_MINUTE_INTERVAL;
public:
	string mMap;
	int mPosX;
	int mPosY;
	int mID;
	string mMonsterLabel;
	int mRadius;
	int mCount;
	int mMinuteInterval;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mMap = reader->getString(table->getCol(COL_MAP));
		mPosX = reader->getInt(table->getCol(COL_POSX));
		mPosY = reader->getInt(table->getCol(COL_POSY));
		mID = reader->getInt(table->getCol(COL_ID));
		mMonsterLabel = reader->getString(table->getCol(COL_MONSTER_LABEL));
		mRadius = reader->getInt(table->getCol(COL_RADIUS));
		mCount = reader->getInt(table->getCol(COL_COUNT));
		mMinuteInterval = reader->getInt(table->getCol(COL_MINUTE_INTERVAL));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueString(valueString, mMap);
		StringUtility::appendValueInt(valueString, mPosX);
		StringUtility::appendValueInt(valueString, mPosY);
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mMonsterLabel);
		StringUtility::appendValueInt(valueString, mRadius);
		StringUtility::appendValueInt(valueString, mCount);
		StringUtility::appendValueInt(valueString, mMinuteInterval);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateString(updateString, COL_MAP, mMap);
		StringUtility::appendUpdateInt(updateString, COL_POSX, mPosX);
		StringUtility::appendUpdateInt(updateString, COL_POSY, mPosY);
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_MONSTER_LABEL, mMonsterLabel);
		StringUtility::appendUpdateInt(updateString, COL_RADIUS, mRadius);
		StringUtility::appendUpdateInt(updateString, COL_COUNT, mCount);
		StringUtility::appendUpdateInt(updateString, COL_MINUTE_INTERVAL, mMinuteInterval);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_MAP);
		table->registeColumn(COL_POSX);
		table->registeColumn(COL_POSY);
		table->registeColumn(COL_ID);
		table->registeColumn(COL_MONSTER_LABEL);
		table->registeColumn(COL_RADIUS);
		table->registeColumn(COL_COUNT);
		table->registeColumn(COL_MINUTE_INTERVAL);
	}
};

class SQLiteMonGen : public SQLiteTable
{
public:
	SQLiteMonGen(SQLite* sqlite)
		:SQLiteTable("MonGen", sqlite)
	{
		MonGenData::registeColumn(this);
	}
	bool query(const MonGenData& data);
	void query(string map, txVector<MonGenData>& dataList);
	void queryAll(txVector<MonGenData>& dataList);
	bool insert(const MonGenData& data);
};

#endif