#ifndef _SQLITE_MONSTER_FRAME_H_
#define _SQLITE_MONSTER_FRAME_H_

#include "SQLiteTable.h"

class MonsterFrameData
{
public:
	int mID;
	int mDirection;
	std::string mAction;
	int mFrameCount;
	txVector<float> mPosX;
	txVector<float> mPosY;
	std::string mPrefab;
};

class SQLiteMonsterFrame : SQLiteTable
{
	std::string COL_ID = "MonsterID";
	std::string COL_DIRECTION = "Direction";
	std::string COL_ACTION = "Action";
	std::string COL_FRAME_COUNT = "FrameCount";
	std::string COL_POSX = "PosX";
	std::string COL_POSY = "PosY";
	std::string COL_PREFAB = "Prefab";
public:
	SQLiteMonsterFrame(SQLite* sqlite)
		:SQLiteTable("MonsterFrame", sqlite)
	{
		registeColumn(COL_ID);
		registeColumn(COL_DIRECTION);
		registeColumn(COL_ACTION);
		registeColumn(COL_FRAME_COUNT);
		registeColumn(COL_POSX);
		registeColumn(COL_POSY);
		registeColumn(COL_PREFAB);
	}
	void query(int monsterID, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, txVector<MonsterFrameData>& dataList);
	void query(int monsterID, int direction, const std::string& action, txVector<MonsterFrameData>& dataList);
	bool updateData(const MonsterFrameData& data);
	bool insert(const MonsterFrameData& data);
	//--------------------------------------------------------------------------------------------------------------------
protected:
	void parseReader(SQLiteDataReader* reader, txVector<MonsterFrameData>& dataList);
};

#endif