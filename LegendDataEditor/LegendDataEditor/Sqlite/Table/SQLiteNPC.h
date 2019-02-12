#ifndef _SQLITE_NPC_H_
#define _SQLITE_NPC_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class NPCData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_SCRIPT;
	static string COL_MAP;
	static string COL_POSX;
	static string COL_POSY;
	static string COL_LABEL;
	static string COL_FLAG;
	static string COL_APPERANCE;
public:
	int mID;
	string mScript;
	string mMap;
	int mPosX;
	int mPosY;
	string mLabel;
	int mFlag;
	int mApperance;
public:
	NPCData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mScript, COL_SCRIPT);
		REGISTE_PARAM(mMap, COL_MAP);
		REGISTE_PARAM(mPosX, COL_POSX);
		REGISTE_PARAM(mPosY, COL_POSY);
		REGISTE_PARAM(mLabel, COL_LABEL);
		REGISTE_PARAM(mFlag, COL_FLAG);
		REGISTE_PARAM(mApperance, COL_APPERANCE);
	}
};

class SQLiteNPC : public SQLiteTable
{
public:
	SQLiteNPC(SQLite* sqlite)
		:SQLiteTable("NPC", sqlite)
	{
		;
	}
	void query(txVector<NPCData>& dataList);
	bool insert(const NPCData& data);
};

#endif