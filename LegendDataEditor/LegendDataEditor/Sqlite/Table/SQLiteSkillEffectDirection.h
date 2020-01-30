#ifndef _SQLITE_SKILL_EFFECT_DIRECTION_H_
#define _SQLITE_SKILL_EFFECT_DIRECTION_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class SkillEffectDirectionData : public SQLiteTableData
{
public:
	static string ID;
	static string PREFAB;
	static string DIRECTION_COUNT;
public:
	int mID;
	string mPrefab;
	int mDirectionCount;
public:
	SkillEffectDirectionData()
	{
		REGISTE_PARAM(mID, ID);
		REGISTE_PARAM(mPrefab, PREFAB);
		REGISTE_PARAM(mDirectionCount, DIRECTION_COUNT);
	}
};

class SQLiteSkillEffectDirection : public SQLiteTable
{
public:
	SQLiteSkillEffectDirection(SQLite* sqlite)
		:SQLiteTable("SkillEffectDirection", sqlite){}
};

#endif