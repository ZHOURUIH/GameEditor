#ifndef _SQLITE_EQUIP_FRAME_H_
#define _SQLITE_EQUIP_FRAME_H_

#include "SQLiteTable.h"

class EquipFrameData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_LABEL;
	static string COL_DIRECTION;
	static string COL_ACTION;
	static string COL_FRAME_COUNT;
	static string COL_POSX;
	static string COL_POSY;
public:
	int mID;
	string mLabel;
	int mDirection;
	string mAction;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
public:
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mID = reader->getInt(table->getCol(COL_ID));
		mLabel = reader->getString(table->getCol(COL_LABEL));
		mDirection = reader->getInt(table->getCol(COL_DIRECTION));
		mAction = reader->getString(table->getCol(COL_ACTION));
		mFrameCount = reader->getInt(table->getCol(COL_FRAME_COUNT));
		StringUtility::stringToIntArray(reader->getString(table->getCol(COL_POSX)), mPosX);
		StringUtility::stringToIntArray(reader->getString(table->getCol(COL_POSY)), mPosY);
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mLabel);
		StringUtility::appendValueInt(valueString, mDirection);
		StringUtility::appendValueString(valueString, mAction);
		StringUtility::appendValueInt(valueString, mFrameCount);
		StringUtility::appendValueIntArray(valueString, mPosX);
		StringUtility::appendValueIntArray(valueString, mPosY);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_LABEL, mLabel);
		StringUtility::appendUpdateInt(updateString, COL_DIRECTION, mDirection);
		StringUtility::appendUpdateString(updateString, COL_ACTION, mAction);
		StringUtility::appendUpdateInt(updateString, COL_FRAME_COUNT, mFrameCount);
		StringUtility::appendUpdateIntArray(updateString, COL_POSX, mPosX);
		StringUtility::appendUpdateIntArray(updateString, COL_POSY, mPosY);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ID);
		table->registeColumn(COL_LABEL);
		table->registeColumn(COL_DIRECTION);
		table->registeColumn(COL_ACTION);
		table->registeColumn(COL_FRAME_COUNT);
		table->registeColumn(COL_POSX);
		table->registeColumn(COL_POSY);
	}
};

class SQLiteEquipFrame : public SQLiteTable
{
public:
	SQLiteEquipFrame(SQLite* sqlite)
		:SQLiteTable("EquipFrame", sqlite)
	{
		EquipFrameData::registeColumn(this);
	}
	void query(int id, txVector<EquipFrameData>& dataList);
	void query(int id, int direction, txVector<EquipFrameData>& dataList);
	void query(int id, int direction, const string& action, txVector<EquipFrameData>& dataList);
	bool updateData(const EquipFrameData& data);
	bool insert(const EquipFrameData& data);
};

#endif