#ifndef _SQLITE_MONSTER_FRAME_H_
#define _SQLITE_MONSTER_FRAME_H_

#include "SQLiteTable.h"

class MonsterFrameData : public SQLiteTableData
{
public:
	static std::string COL_ID;
	static std::string COL_LABEL;
	static std::string COL_DIRECTION;
	static std::string COL_ACTION;
	static std::string COL_FRAME_COUNT;
	static std::string COL_POSX;
	static std::string COL_POSY;
	int mID;
	std::string mLabel;
	int mDirection;
	std::string mAction;
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

class SQLiteMonsterFrame : SQLiteTable
{
public:
	SQLiteMonsterFrame(SQLite* sqlite)
		:SQLiteTable("MonsterFrame", sqlite)
	{
		MonsterFrameData::registeColumn(this);
	}
	void query(int monsterID, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, const std::string& action, txVector<MonsterFrameData>& dataList);
	bool updateData(const MonsterFrameData& data);
	bool insert(const MonsterFrameData& data);
};

#endif