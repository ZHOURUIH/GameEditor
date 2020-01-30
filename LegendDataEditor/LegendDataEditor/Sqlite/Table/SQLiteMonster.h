#ifndef _SQLITE_MONSTER_H_
#define _SQLITE_MONSTER_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class MonsterData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_DESCRIBE;
	static string COL_TEMPLATE_ID;
	static string COL_LEVEL;
	static string COL_HP;
	static string COL_MIN_PHYSICS_DEFENCE;
	static string COL_MAX_PHYSICS_DEFENCE;
	static string COL_MIN_MAGIC_DEFENCE;
	static string COL_MAX_MAGIC_DEFENCE;
	static string COL_MIN_PHYSICS_ATTACK;
	static string COL_MAX_PHYSICS_ATTACK;
	static string COL_MIN_MAGIC_ATTACK;
	static string COL_MAX_MAGIC_ATTACK;
	static string COL_MIN_SORCERY_ATTACK;
	static string COL_MAX_SORCERY_ATTACK;
	static string COL_MOVE_SPEED;
	static string COL_HIT_PROBABLITY;
	static string COL_EVADE_PROBABLITY;
	static string COL_WALK_SPEED;
	static string COL_WALK_STEP;
	static string COL_WALK_WAIT;
	static string COL_ATTACK_SPEED;
	static string COL_EXP;
	static string COL_DROP_LIST;
	static string COL_SKILL;
	static string COL_SKILL_LEVEL;
public:
	int mID;
	string mDescribe;
	int mTemplateID;
	int mLevel;
	int mHP;
	int mMinPhysicsDefence;
	int mMaxPhysicsDefence;
	int mMinMagicDefence;
	int mMaxMagicDefence;
	int mMinPhysicsAttack;
	int mMaxPhysicsAttack;
	int mMinMagicAttack;
	int mMaxMagicAttack;
	int mMinSorceryAttack;
	int mMaxSorceryAttack;
	int mMoveSpeed;
	int mHitProbability;
	int mEvadeProbability;
	int mWalkSpeed;		// 每次移动的间隔,毫秒
	int mWalkStep;		// 一次移动的步数
	int mWalkWait;		// 移动每步的等待
	int mAttackSpeed;	// 攻击间隔,毫秒
	int mExp;
	string mDropList;
	txVector<int> mSkill;
	txVector<int> mSkillLevel;
public:
	MonsterData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mDescribe, COL_DESCRIBE);
		REGISTE_PARAM(mTemplateID, COL_TEMPLATE_ID);
		REGISTE_PARAM(mLevel, COL_LEVEL);
		REGISTE_PARAM(mHP, COL_HP);
		REGISTE_PARAM(mMinPhysicsDefence, COL_MIN_PHYSICS_DEFENCE);
		REGISTE_PARAM(mMaxPhysicsDefence, COL_MAX_PHYSICS_DEFENCE);
		REGISTE_PARAM(mMinMagicDefence, COL_MIN_MAGIC_DEFENCE);
		REGISTE_PARAM(mMaxMagicDefence, COL_MAX_MAGIC_DEFENCE);
		REGISTE_PARAM(mMinPhysicsAttack, COL_MIN_PHYSICS_ATTACK);
		REGISTE_PARAM(mMaxPhysicsAttack, COL_MAX_PHYSICS_ATTACK);
		REGISTE_PARAM(mMinMagicAttack, COL_MIN_MAGIC_ATTACK);
		REGISTE_PARAM(mMaxMagicAttack, COL_MAX_MAGIC_ATTACK);
		REGISTE_PARAM(mMinSorceryAttack, COL_MIN_SORCERY_ATTACK);
		REGISTE_PARAM(mMaxSorceryAttack, COL_MAX_SORCERY_ATTACK);
		REGISTE_PARAM(mMoveSpeed, COL_MOVE_SPEED);
		REGISTE_PARAM(mHitProbability, COL_HIT_PROBABLITY);
		REGISTE_PARAM(mEvadeProbability, COL_EVADE_PROBABLITY);
		REGISTE_PARAM(mWalkSpeed, COL_WALK_SPEED);
		REGISTE_PARAM(mWalkStep, COL_WALK_STEP);
		REGISTE_PARAM(mWalkWait, COL_WALK_WAIT);
		REGISTE_PARAM(mAttackSpeed, COL_ATTACK_SPEED);
		REGISTE_PARAM(mExp, COL_EXP);
		REGISTE_PARAM(mDropList, COL_DROP_LIST);
		REGISTE_PARAM(mSkill, COL_SKILL);
		REGISTE_PARAM(mSkillLevel, COL_SKILL_LEVEL);
	}
};

class SQLiteMonster : public SQLiteTable
{
public:
	SQLiteMonster(SQLite* sqlite)
		:SQLiteTable("Monster", sqlite){}
	void query(int id, MonsterData& data);
	bool insert(const MonsterData& data);
	bool update(const MonsterData& data);
};

#endif