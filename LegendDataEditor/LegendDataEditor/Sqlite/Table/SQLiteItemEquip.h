#ifndef _SQLITE_ITEM_EQUIP_H_
#define _SQLITE_ITEM_EQUIP_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ItemEquipData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_DESCRIPTION;
	static string COL_TYPE;
	static string COL_SHAPE;
	static string COL_JOB;
	static string COL_SEX;
	static string COL_LEVEL;
	static string COL_ANI_COUNT;
	static string COL_LOOKS;
	static string COL_DURA_MAX;
	static string COL_MIN_ARMOR;
	static string COL_MAX_ARMOR;
	static string COL_MIN_MAGIC_ARMOR;
	static string COL_MAX_MAGIC_ARMOR;
	static string COL_MIN_DAMAGE;
	static string COL_MAX_DAMAGE;
	static string COL_MIN_MAGIC;
	static string COL_MAX_MAGIC;
	static string COL_MIN_SORCERY;
	static string COL_MAX_SORCERY;
	static string COL_LUCKY;
	static string COL_HIT_PROBABILITY;
	static string COL_EVADE_PROBABILITY;
	static string COL_WEIGHT;
	static string COL_PRICE;
public:
	int mID;
	string mName;
	string mDescription;
	int mType;
	int mShape;
	int mJob;
	int mSex;
	int mLevel;
	int mAniCount;
	int mLooks;
	int mDuraMax;
	int mMinArmor;
	int mMaxArmor;
	int mMinMagicArmor;
	int mMaxMagicArmor;
	int mMinDamage;
	int mMaxDamage;
	int mMinMagic;
	int mMaxMagic;
	int mMinSorcery;
	int mMaxSorcery;
	int mLucky;
	int mHitProbability;
	int mEvadeProbability;
	int mWeight;
	int mPrice;
public:
	ItemEquipData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mDescription, COL_DESCRIPTION);
		REGISTE_PARAM(mType, COL_TYPE);
		REGISTE_PARAM(mShape, COL_SHAPE);
		REGISTE_PARAM(mJob, COL_JOB);
		REGISTE_PARAM(mSex, COL_SEX);
		REGISTE_PARAM(mLevel, COL_LEVEL);
		REGISTE_PARAM(mAniCount, COL_ANI_COUNT);
		REGISTE_PARAM(mLooks, COL_LOOKS);
		REGISTE_PARAM(mDuraMax, COL_DURA_MAX);
		REGISTE_PARAM(mMinArmor, COL_MIN_ARMOR);
		REGISTE_PARAM(mMaxArmor, COL_MAX_ARMOR);
		REGISTE_PARAM(mMinMagicArmor, COL_MIN_MAGIC_ARMOR);
		REGISTE_PARAM(mMaxMagicArmor, COL_MAX_MAGIC_ARMOR);
		REGISTE_PARAM(mMinDamage, COL_MIN_DAMAGE);
		REGISTE_PARAM(mMaxDamage, COL_MAX_DAMAGE);
		REGISTE_PARAM(mMinMagic, COL_MIN_MAGIC);
		REGISTE_PARAM(mMaxMagic, COL_MAX_MAGIC);
		REGISTE_PARAM(mMinSorcery, COL_MIN_SORCERY);
		REGISTE_PARAM(mMaxSorcery, COL_MAX_SORCERY);
		REGISTE_PARAM(mLucky, COL_LUCKY);
		REGISTE_PARAM(mHitProbability, COL_HIT_PROBABILITY);
		REGISTE_PARAM(mEvadeProbability, COL_EVADE_PROBABILITY);
		REGISTE_PARAM(mWeight, COL_WEIGHT);
		REGISTE_PARAM(mPrice, COL_PRICE);
	}
};

class SQLiteItemEquip : public SQLiteTable
{
public:
	SQLiteItemEquip(SQLite* sqlite)
		:SQLiteTable("ItemEquip", sqlite)
	{}
	bool query(int id, ItemEquipData& data);
	bool query(const string& name, ItemEquipData& data);
	bool insert(const ItemEquipData& data);
	bool update(const ItemEquipData& data);
};

#endif