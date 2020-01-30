#ifndef _SQLITE_MONSTER_TEMPLATE_H_
#define _SQLITE_MONSTER_TEMPLATE_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MonsterTemplateData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_RACE;
	static string COL_RACE_IMAGE;
	static string COL_MODEL;
	static string COL_APPERANCE;
	static string COL_MODEL_RADIUS;
	static string COL_MONSTER_RACE;
	static string COL_COOL_EYE;
public:
	int mID;
	string mName;
	int mRace;			// 正常时的外观
	int mRaceImage;		// 攻击效果
	string mModel;
	int mApperance;
	int mModelRadius;
	int mMonsterRace;	// 怪物种族,对应MONTER_RACE枚举
	int mCoolEye;		// 是否主动攻击
public:
	MonsterTemplateData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mRace, COL_RACE);
		REGISTE_PARAM(mRaceImage, COL_RACE_IMAGE);
		REGISTE_PARAM(mModel, COL_MODEL);
		REGISTE_PARAM(mApperance, COL_APPERANCE);
		REGISTE_PARAM(mModelRadius, COL_MODEL_RADIUS);
		REGISTE_PARAM(mMonsterRace, COL_MONSTER_RACE);
		REGISTE_PARAM(mCoolEye, COL_COOL_EYE);
	}
};

class SQLiteMonsterTemplate : public SQLiteTable
{
public:
	SQLiteMonsterTemplate(SQLite* sqlite)
		:SQLiteTable("MonsterTemplate", sqlite){}
	void query(int id, MonsterTemplateData& data);
	bool insert(const MonsterTemplateData& data);
	bool update(const MonsterTemplateData& data);
};

#endif