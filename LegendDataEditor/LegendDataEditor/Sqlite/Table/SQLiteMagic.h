#ifndef _SQLITE_MAGIC_H_
#define _SQLITE_MAGIC_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MagicData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_EFFECT_TYPE;
	static string COL_EFFECT;
	static string COL_SPELL;
	static string COL_POWER;
	static string COL_MAX_POWER;
	static string COL_DEF_SPELL;
	static string COL_DEF_POWER;
	static string COL_DEF_MAX_POWER;
	static string COL_JOB;
	static string COL_NEED_LEVEL_1;
	static string COL_LEVEL_TRAIN_1;
	static string COL_NEED_LEVEL_2;
	static string COL_LEVEL_TRAIN_2;
	static string COL_NEED_LEVEL_3;
	static string COL_LEVEL_TRAIN_3;
	static string COL_DELAY;
	static string COL_DESCRIBE;
public:
	int mID;
	string mName;
	int mEffectType;	// 效果类型（使用技能时角色的动作效果）
	int mEffect;		// 效果（技能产生的动画效果）
	int mSpell;			// 每次耗用魔法值
	int mPower;			// 基本威力
	int mMaxPower;		// 最大威力
	int mDefSpell;		// 升级后增加的每次耗用魔法值
	int mDefPower;		// 升级后增加的威力
	int mDefMaxPower;	// 升级后增加的最大威力
	int mJob;			// 职业（0-战士，1-法师，2-道士）
	int mNeedLevel1;	// 1级技能所需等级
	int mLevelTrain1;	// 1级技能修炼所需经验
	int mNeedLevel2;
	int mLevelTrain2;
	int mNeedLevel3;
	int mLevelTrain3;
	int mDelay;			// 技能延迟时间
	string mDescribe;
public:
	MagicData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mEffectType, COL_EFFECT_TYPE);
		REGISTE_PARAM(mEffect, COL_EFFECT);
		REGISTE_PARAM(mSpell, COL_SPELL);
		REGISTE_PARAM(mPower, COL_POWER);
		REGISTE_PARAM(mMaxPower, COL_MAX_POWER);
		REGISTE_PARAM(mDefSpell, COL_DEF_SPELL);
		REGISTE_PARAM(mDefPower, COL_DEF_POWER);
		REGISTE_PARAM(mDefMaxPower, COL_DEF_MAX_POWER);
		REGISTE_PARAM(mJob, COL_JOB);
		REGISTE_PARAM(mNeedLevel1, COL_NEED_LEVEL_1);
		REGISTE_PARAM(mLevelTrain1, COL_LEVEL_TRAIN_1);
		REGISTE_PARAM(mNeedLevel2, COL_NEED_LEVEL_2);
		REGISTE_PARAM(mLevelTrain2, COL_LEVEL_TRAIN_2);
		REGISTE_PARAM(mNeedLevel3, COL_NEED_LEVEL_3);
		REGISTE_PARAM(mLevelTrain3, COL_LEVEL_TRAIN_3);
		REGISTE_PARAM(mDelay, COL_DELAY);
		REGISTE_PARAM(mDescribe, COL_DESCRIBE);
	}
};

class SQLiteMagic : public SQLiteTable
{
public:
	SQLiteMagic(SQLite* sqlite)
		:SQLiteTable("Magic", sqlite)
	{
		;
	}
	void query(const string& name, MagicData& data);
	bool insert(const MagicData& data);
	bool update(const MagicData& data);
};

#endif