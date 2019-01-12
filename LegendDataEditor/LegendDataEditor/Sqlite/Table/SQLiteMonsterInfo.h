#ifndef _SQLITE_MONSTER_INFO_H_
#define _SQLITE_MONSTER_INFO_H_

#include "SQLiteTable.h"

class MonsterInfoData : public SQLiteTableData
{
public:
	static string COL_NAME;
	static string COL_RACE;
	static string COL_RACE_IMAGE;
	static string COL_APPERANCE;
	static string COL_LEVEL;
	static string COL_UNDEAD;
	static string COL_COOL_EYE;
	static string COL_HP;
	static string COL_MP;
	static string COL_ARMOR_CLASS;
	static string COL_MAGIC_ARMOR_CLASS;
	static string COL_DAMAGE_CLASS;
	static string COL_MAX_DAMAGE_CLASS;
	static string COL_MAGIC_CLASS;
	static string COL_SC;
	static string COL_SPEED;
	static string COL_HIT;
	static string COL_WALK_SPEED;
	static string COL_WALK_STEP;
	static string COL_WALK_WAIT;
	static string COL_ATTACK_SPEED;
	static string COL_EXP;
public:
	string mName;
	int mRace;			// 正常时的外观
	int mRaceImage;		// 攻击效果
	int mApperance;
	int mLevel;
	int mUndead;		// 是否为不死系,不死系不可召唤
	int mCoolEye;		// 是否主动攻击
	int mHP;
	int mMP;
	int mArmorClass;	// 防御力
	int mMagicArmorClass;// 魔法防御力
	int mDamageClass;	// 攻击力
	int mMaxDamageClass;// 最大攻击力
	int mMagicClass;	// 魔法攻击力
	int mSC;			// 道术攻击力
	int mSpeed;			// 敏捷
	int mHit;			// 命中率
	int mWalkSpeed;		// 每次移动的间隔,毫秒
	int mWalkStep;		// 一次移动的步数
	int mWalkWait;		// 移动每步的等待
	int mAttackSpeed;	// 攻击间隔,毫秒
	int mExp;
public:
	MonsterInfoData()
	{
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mRace, COL_RACE);
		REGISTE_PARAM(mRaceImage, COL_RACE_IMAGE);
		REGISTE_PARAM(mApperance, COL_APPERANCE);
		REGISTE_PARAM(mLevel, COL_LEVEL);
		REGISTE_PARAM(mUndead, COL_UNDEAD);
		REGISTE_PARAM(mCoolEye, COL_COOL_EYE);
		REGISTE_PARAM(mHP, COL_HP);
		REGISTE_PARAM(mMP, COL_MP);
		REGISTE_PARAM(mArmorClass, COL_ARMOR_CLASS);
		REGISTE_PARAM(mMagicArmorClass, COL_MAGIC_ARMOR_CLASS);
		REGISTE_PARAM(mDamageClass, COL_DAMAGE_CLASS);
		REGISTE_PARAM(mMaxDamageClass, COL_MAX_DAMAGE_CLASS);
		REGISTE_PARAM(mMagicClass, COL_MAGIC_CLASS);
		REGISTE_PARAM(mSC, COL_SC);
		REGISTE_PARAM(mSpeed, COL_SPEED);
		REGISTE_PARAM(mHit, COL_HIT);
		REGISTE_PARAM(mWalkSpeed, COL_WALK_SPEED);
		REGISTE_PARAM(mWalkStep, COL_WALK_STEP);
		REGISTE_PARAM(mWalkWait, COL_WALK_WAIT);
		REGISTE_PARAM(mAttackSpeed, COL_ATTACK_SPEED);
		REGISTE_PARAM(mExp, COL_EXP);
	}
};

class SQLiteMonsterInfo : public SQLiteTable
{
public:
	SQLiteMonsterInfo(SQLite* sqlite)
		:SQLiteTable("MonsterInfo", sqlite)
	{
		;
	}
	void query(const string& name, MonsterInfoData& data);
	void queryAll(txVector<MonsterInfoData>& dataList);
	bool insert(const MonsterInfoData& data);
	bool update(const MonsterInfoData& data);
};

#endif