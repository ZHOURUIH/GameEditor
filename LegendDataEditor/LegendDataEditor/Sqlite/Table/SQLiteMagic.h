#ifndef _SQLITE_MAGIC_H_
#define _SQLITE_MAGIC_H_

#include "SQLiteTable.h"

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
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mID = reader->getInt(table->getCol(COL_ID));
		mName = reader->getString(table->getCol(COL_NAME));
		mEffectType = reader->getInt(table->getCol(COL_EFFECT_TYPE));
		mEffect = reader->getInt(table->getCol(COL_EFFECT));
		mSpell = reader->getInt(table->getCol(COL_SPELL));
		mPower = reader->getInt(table->getCol(COL_POWER));
		mMaxPower = reader->getInt(table->getCol(COL_MAX_POWER));
		mDefSpell = reader->getInt(table->getCol(COL_DEF_SPELL));
		mDefPower = reader->getInt(table->getCol(COL_DEF_POWER));
		mDefMaxPower = reader->getInt(table->getCol(COL_DEF_MAX_POWER));
		mJob = reader->getInt(table->getCol(COL_JOB));
		mNeedLevel1 = reader->getInt(table->getCol(COL_NEED_LEVEL_1));
		mLevelTrain1 = reader->getInt(table->getCol(COL_LEVEL_TRAIN_1));
		mNeedLevel2 = reader->getInt(table->getCol(COL_NEED_LEVEL_2));
		mLevelTrain2 = reader->getInt(table->getCol(COL_LEVEL_TRAIN_2));
		mNeedLevel3 = reader->getInt(table->getCol(COL_NEED_LEVEL_3));
		mLevelTrain3 = reader->getInt(table->getCol(COL_LEVEL_TRAIN_3));
		mDelay = reader->getInt(table->getCol(COL_DELAY));
		mDescribe = reader->getInt(table->getCol(COL_DESCRIBE));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueInt(valueString, mID);
		StringUtility::appendValueString(valueString, mName);
		StringUtility::appendValueInt(valueString, mEffectType);
		StringUtility::appendValueInt(valueString, mEffect);
		StringUtility::appendValueInt(valueString, mSpell);
		StringUtility::appendValueInt(valueString, mPower);
		StringUtility::appendValueInt(valueString, mMaxPower);
		StringUtility::appendValueInt(valueString, mDefSpell);
		StringUtility::appendValueInt(valueString, mDefPower);
		StringUtility::appendValueInt(valueString, mDefMaxPower);
		StringUtility::appendValueInt(valueString, mJob);
		StringUtility::appendValueInt(valueString, mNeedLevel1);
		StringUtility::appendValueInt(valueString, mLevelTrain1);
		StringUtility::appendValueInt(valueString, mNeedLevel2);
		StringUtility::appendValueInt(valueString, mLevelTrain2);
		StringUtility::appendValueInt(valueString, mNeedLevel3);
		StringUtility::appendValueInt(valueString, mLevelTrain3);
		StringUtility::appendValueInt(valueString, mDelay);
		StringUtility::appendValueString(valueString, mDescribe);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateInt(updateString, COL_ID, mID);
		StringUtility::appendUpdateString(updateString, COL_NAME, mName);
		StringUtility::appendUpdateInt(updateString, COL_EFFECT_TYPE, mEffectType);
		StringUtility::appendUpdateInt(updateString, COL_EFFECT, mEffect);
		StringUtility::appendUpdateInt(updateString, COL_SPELL, mSpell);
		StringUtility::appendUpdateInt(updateString, COL_POWER, mPower);
		StringUtility::appendUpdateInt(updateString, COL_MAX_POWER, mMaxPower);
		StringUtility::appendUpdateInt(updateString, COL_DEF_SPELL, mDefSpell);
		StringUtility::appendUpdateInt(updateString, COL_DEF_POWER, mDefPower);
		StringUtility::appendUpdateInt(updateString, COL_DEF_MAX_POWER, mDefMaxPower);
		StringUtility::appendUpdateInt(updateString, COL_JOB, mJob);
		StringUtility::appendUpdateInt(updateString, COL_NEED_LEVEL_1, mNeedLevel1);
		StringUtility::appendUpdateInt(updateString, COL_LEVEL_TRAIN_1, mLevelTrain1);
		StringUtility::appendUpdateInt(updateString, COL_NEED_LEVEL_2, mNeedLevel2);
		StringUtility::appendUpdateInt(updateString, COL_LEVEL_TRAIN_2, mLevelTrain2);
		StringUtility::appendUpdateInt(updateString, COL_NEED_LEVEL_3, mNeedLevel3);
		StringUtility::appendUpdateInt(updateString, COL_LEVEL_TRAIN_3, mLevelTrain3);
		StringUtility::appendUpdateInt(updateString, COL_DELAY, mDelay);
		StringUtility::appendUpdateString(updateString, COL_DESCRIBE, mDescribe);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_ID);
		table->registeColumn(COL_NAME);
		table->registeColumn(COL_EFFECT_TYPE);
		table->registeColumn(COL_EFFECT);
		table->registeColumn(COL_SPELL);
		table->registeColumn(COL_POWER);
		table->registeColumn(COL_MAX_POWER);
		table->registeColumn(COL_DEF_SPELL);
		table->registeColumn(COL_DEF_POWER);
		table->registeColumn(COL_DEF_MAX_POWER);
		table->registeColumn(COL_JOB);
		table->registeColumn(COL_NEED_LEVEL_1);
		table->registeColumn(COL_LEVEL_TRAIN_1);
		table->registeColumn(COL_NEED_LEVEL_2);
		table->registeColumn(COL_LEVEL_TRAIN_2);
		table->registeColumn(COL_NEED_LEVEL_3);
		table->registeColumn(COL_LEVEL_TRAIN_3);
		table->registeColumn(COL_DELAY);
		table->registeColumn(COL_DESCRIBE);
	}
};

class SQLiteMagic : public SQLiteTable
{
public:
	SQLiteMagic(SQLite* sqlite)
		:SQLiteTable("Magic", sqlite)
	{
		MagicData::registeColumn(this);
	}
	void query(const string& name, MagicData& data);
	void queryAll(txVector<MagicData>& dataList);
	bool insert(const MagicData& data);
	bool update(const MagicData& data);
};

#endif