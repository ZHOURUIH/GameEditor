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
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mName = reader->getString(table->getCol(COL_NAME));
		mRace = reader->getInt(table->getCol(COL_RACE));
		mRaceImage = reader->getInt(table->getCol(COL_RACE_IMAGE));
		mApperance = reader->getInt(table->getCol(COL_APPERANCE));
		mLevel = reader->getInt(table->getCol(COL_LEVEL));
		mUndead = reader->getInt(table->getCol(COL_UNDEAD));
		mCoolEye = reader->getInt(table->getCol(COL_COOL_EYE));
		mHP = reader->getInt(table->getCol(COL_HP));
		mMP = reader->getInt(table->getCol(COL_MP));
		mArmorClass = reader->getInt(table->getCol(COL_ARMOR_CLASS));
		mMagicArmorClass = reader->getInt(table->getCol(COL_MAGIC_ARMOR_CLASS));
		mDamageClass = reader->getInt(table->getCol(COL_DAMAGE_CLASS));
		mMaxDamageClass = reader->getInt(table->getCol(COL_MAX_DAMAGE_CLASS));
		mMagicClass = reader->getInt(table->getCol(COL_MAGIC_CLASS));
		mSC = reader->getInt(table->getCol(COL_SC));
		mSpeed = reader->getInt(table->getCol(COL_SPEED));
		mHit = reader->getInt(table->getCol(COL_HIT));
		mWalkSpeed = reader->getInt(table->getCol(COL_WALK_SPEED));
		mWalkStep = reader->getInt(table->getCol(COL_WALK_STEP));
		mWalkWait = reader->getInt(table->getCol(COL_WALK_WAIT));
		mAttackSpeed = reader->getInt(table->getCol(COL_ATTACK_SPEED));
		mExp = reader->getInt(table->getCol(COL_EXP));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueString(valueString, mName);
		StringUtility::appendValueInt(valueString, mRace);
		StringUtility::appendValueInt(valueString, mRaceImage);
		StringUtility::appendValueInt(valueString, mApperance);
		StringUtility::appendValueInt(valueString, mLevel);
		StringUtility::appendValueInt(valueString, mUndead);
		StringUtility::appendValueInt(valueString, mCoolEye);
		StringUtility::appendValueInt(valueString, mHP);
		StringUtility::appendValueInt(valueString, mMP);
		StringUtility::appendValueInt(valueString, mArmorClass);
		StringUtility::appendValueInt(valueString, mMagicArmorClass);
		StringUtility::appendValueInt(valueString, mDamageClass);
		StringUtility::appendValueInt(valueString, mMaxDamageClass);
		StringUtility::appendValueInt(valueString, mMagicClass);
		StringUtility::appendValueInt(valueString, mSC);
		StringUtility::appendValueInt(valueString, mSpeed);
		StringUtility::appendValueInt(valueString, mHit);
		StringUtility::appendValueInt(valueString, mWalkSpeed);
		StringUtility::appendValueInt(valueString, mWalkStep);
		StringUtility::appendValueInt(valueString, mWalkWait);
		StringUtility::appendValueInt(valueString, mAttackSpeed);
		StringUtility::appendValueInt(valueString, mExp);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateString(updateString, COL_NAME, mName);
		StringUtility::appendUpdateInt(updateString, COL_RACE, mRace);
		StringUtility::appendUpdateInt(updateString, COL_RACE_IMAGE, mRaceImage);
		StringUtility::appendUpdateInt(updateString, COL_APPERANCE, mApperance);
		StringUtility::appendUpdateInt(updateString, COL_LEVEL, mLevel);
		StringUtility::appendUpdateInt(updateString, COL_UNDEAD, mUndead);
		StringUtility::appendUpdateInt(updateString, COL_COOL_EYE, mCoolEye);
		StringUtility::appendUpdateInt(updateString, COL_HP, mHP);
		StringUtility::appendUpdateInt(updateString, COL_MP, mMP);
		StringUtility::appendUpdateInt(updateString, COL_ARMOR_CLASS, mArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_MAGIC_ARMOR_CLASS, mMagicArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_DAMAGE_CLASS, mDamageClass);
		StringUtility::appendUpdateInt(updateString, COL_MAX_DAMAGE_CLASS, mMaxDamageClass);
		StringUtility::appendUpdateInt(updateString, COL_MAGIC_CLASS, mMagicClass);
		StringUtility::appendUpdateInt(updateString, COL_SC, mSC);
		StringUtility::appendUpdateInt(updateString, COL_SPEED, mSpeed);
		StringUtility::appendUpdateInt(updateString, COL_HIT, mHit);
		StringUtility::appendUpdateInt(updateString, COL_WALK_SPEED, mWalkSpeed);
		StringUtility::appendUpdateInt(updateString, COL_WALK_STEP, mWalkStep);
		StringUtility::appendUpdateInt(updateString, COL_WALK_WAIT, mWalkWait);
		StringUtility::appendUpdateInt(updateString, COL_ATTACK_SPEED, mAttackSpeed);
		StringUtility::appendUpdateInt(updateString, COL_EXP, mExp);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_NAME);
		table->registeColumn(COL_RACE);
		table->registeColumn(COL_RACE_IMAGE);
		table->registeColumn(COL_APPERANCE);
		table->registeColumn(COL_LEVEL);
		table->registeColumn(COL_UNDEAD);
		table->registeColumn(COL_COOL_EYE);
		table->registeColumn(COL_HP);
		table->registeColumn(COL_MP);
		table->registeColumn(COL_ARMOR_CLASS);
		table->registeColumn(COL_MAGIC_ARMOR_CLASS);
		table->registeColumn(COL_DAMAGE_CLASS);
		table->registeColumn(COL_MAX_DAMAGE_CLASS);
		table->registeColumn(COL_MAGIC_CLASS);
		table->registeColumn(COL_SC);
		table->registeColumn(COL_SPEED);
		table->registeColumn(COL_HIT);
		table->registeColumn(COL_WALK_SPEED);
		table->registeColumn(COL_WALK_STEP);
		table->registeColumn(COL_WALK_WAIT);
		table->registeColumn(COL_ATTACK_SPEED);
		table->registeColumn(COL_EXP);
	}
};

class SQLiteMonsterInfo : public SQLiteTable
{
public:
	SQLiteMonsterInfo(SQLite* sqlite)
		:SQLiteTable("MonsterInfo", sqlite)
	{
		MonsterInfoData::registeColumn(this);
	}
	void query(const string& name, MonsterInfoData& data);
	void queryAll(txVector<MonsterInfoData>& dataList);
	bool insert(const MonsterInfoData& data);
	bool update(const MonsterInfoData& data);
};

#endif