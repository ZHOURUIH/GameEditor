#ifndef _SQLITE_NPC_H_
#define _SQLITE_NPC_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class NPCData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_MAP_ID;
	static string COL_MAP_NAME;
	static string COL_POS;
	static string COL_NAME;
	static string COL_APPERANCE;
	static string COL_STAND_ANIMATION_0;
	static string COL_STAND_ANIMATION_1;
	static string COL_GOODS;
	static string COL_MAIN_TEXT;
public:
	int mID;
	int mMapID;
	string mMapName;
	txVector<int> mPos;
	string mName;
	int mApperance;
	int mStandAnimation0;
	int mStandAnimation1;
	txVector<int> mGoods;
	string mMainText;
public:
	NPCData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mMapID, COL_MAP_ID);
		REGISTE_PARAM(mMapName, COL_MAP_NAME);
		REGISTE_PARAM(mPos, COL_POS);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mApperance, COL_APPERANCE);
		REGISTE_PARAM(mStandAnimation0, COL_STAND_ANIMATION_0);
		REGISTE_PARAM(mStandAnimation1, COL_STAND_ANIMATION_1);
		REGISTE_PARAM(mGoods, COL_GOODS);
		REGISTE_PARAM(mMainText, COL_MAIN_TEXT);
	}
};

class SQLiteNPC : public SQLiteTable
{
public:
	SQLiteNPC(SQLite* sqlite)
		:SQLiteTable("NPC", sqlite){}
	void query(txVector<NPCData*>& dataList);
	bool insert(const NPCData& data);
};

#endif