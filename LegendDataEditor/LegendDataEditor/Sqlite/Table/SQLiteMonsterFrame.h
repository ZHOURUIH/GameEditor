#ifndef _SQLITE_MONSTER_FRAME_H_
#define _SQLITE_MONSTER_FRAME_H_

#include "SQLiteTable.h"

class MonsterFrameData
{
public:
	int mID;
	std::string mLabel;
	int mDirection;
	std::string mAction;
	int mFrameCount;
	txVector<int> mPosX;
	txVector<int> mPosY;
};

class SQLiteMonsterFrame : SQLiteTable
{
	std::string COL_ID = "MonsterID";
	std::string COL_LABEL = "Label";
	std::string COL_DIRECTION = "Direction";
	std::string COL_ACTION = "Action";
	std::string COL_FRAME_COUNT = "FrameCount";
	std::string COL_POSX = "PosX";
	std::string COL_POSY = "PosY";
public:
	SQLiteMonsterFrame(SQLite* sqlite)
		:SQLiteTable("MonsterFrame", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_LABEL);
		registeColumn(COL_DIRECTION);
		registeColumn(COL_ACTION);
		registeColumn(COL_FRAME_COUNT);
		registeColumn(COL_POSX);
		registeColumn(COL_POSY);
	}
	void query(int monsterID, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, const std::string& action, txVector<MonsterFrameData>& dataList);
	bool updateData(const MonsterFrameData& data);
	bool insert(const MonsterFrameData& data);
	bool insertOrUpdate(const MonsterFrameData& data);
	//--------------------------------------------------------------------------------------------------------------------
protected:
	void parseReader(SQLiteDataReader* reader, txVector<MonsterFrameData>& dataList);
};

#endif