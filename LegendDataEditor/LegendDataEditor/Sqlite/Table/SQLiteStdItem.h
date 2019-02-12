#ifndef _SQLITE_STD_ITEM_H_
#define _SQLITE_STD_ITEM_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class StdItemData : public SQLiteTableData
{
public:
	static string COL_INDEX;
	static string COL_NAME;
	static string COL_STD_MODE;
	static string COL_SHAPE;
	static string COL_WEIGHT;
	static string COL_ANI_COUNT;
	static string COL_SOURCE;
	static string COL_RESERVED;
	static string COL_LOOKS;
	static string COL_DURA_MAX;
	static string COL_MIN_ARMOR_CLASS;
	static string COL_MAX_ARMOR_CLASS;
	static string COL_MIN_MAGIC_ARMOR_CLASS;
	static string COL_MAX_MAGIC_ARMOR_CLASS;
	static string COL_MIN_DAMAGE_CLASS;
	static string COL_MAX_DAMAGE_CLASS;
	static string COL_MIN_MAGIC_CLASS;
	static string COL_MAX_MAGIC_CLASS;
	static string COL_MIN_SC;
	static string COL_MAX_SC;
	static string COL_NEED;
	static string COL_NEED_LEVEL;
	static string COL_PRICE;
	static string COL_STOCK;
public:
	int mIndex;
	string mName;
	int mStdMode;			// 分类
	int mShape;				// 外观,用于场景中显示的
	int mWeight;
	int mAniCount;
	int mSource;
	int mReserved;
	int mLooks;				// 显示衣服物品栏里样子和在装备栏里的样子
	int mDuraMax;			// 持久
	int mMinArmorClass;		// 最小防御力
	int mMaxArmorClass;		// 最大防御力
	int mMinMagicArmorClass;// 最小魔法防御力
	int mMaxMagicArmorClass;// 最大魔法防御力
	int mMinDamageClass;	// 最小攻击力
	int mMaxDamageClass;	// 最大攻击力
	int mMinMagicClass;		// 最小魔法攻击力
	int mMaxMagicClass;		// 最大魔法攻击力
	int mMinSC;				// 最小道术攻击力
	int mMaxSC;				// 最大道术攻击力
	int mNeed;				// 附加属性
	int mNeedLevel;			// 需要等级
	int mPrice;
	int mStock;
public:
	StdItemData()
	{
		REGISTE_PARAM(mIndex, COL_INDEX);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mStdMode, COL_STD_MODE);
		REGISTE_PARAM(mShape, COL_SHAPE);
		REGISTE_PARAM(mWeight, COL_WEIGHT);
		REGISTE_PARAM(mAniCount, COL_ANI_COUNT);
		REGISTE_PARAM(mSource, COL_SOURCE);
		REGISTE_PARAM(mReserved, COL_RESERVED);
		REGISTE_PARAM(mLooks, COL_LOOKS);
		REGISTE_PARAM(mDuraMax, COL_DURA_MAX);
		REGISTE_PARAM(mMinArmorClass, COL_MIN_ARMOR_CLASS);
		REGISTE_PARAM(mMaxArmorClass, COL_MAX_ARMOR_CLASS);
		REGISTE_PARAM(mMinMagicArmorClass, COL_MIN_MAGIC_ARMOR_CLASS);
		REGISTE_PARAM(mMaxMagicArmorClass, COL_MAX_MAGIC_ARMOR_CLASS);
		REGISTE_PARAM(mMinDamageClass, COL_MIN_DAMAGE_CLASS);
		REGISTE_PARAM(mMaxDamageClass, COL_MAX_DAMAGE_CLASS);
		REGISTE_PARAM(mMinMagicClass, COL_MIN_MAGIC_CLASS);
		REGISTE_PARAM(mMaxMagicClass, COL_MAX_MAGIC_CLASS);
		REGISTE_PARAM(mMinSC, COL_MIN_SC);
		REGISTE_PARAM(mMaxSC, COL_MAX_SC);
		REGISTE_PARAM(mNeed, COL_NEED);
		REGISTE_PARAM(mNeedLevel, COL_NEED_LEVEL);
		REGISTE_PARAM(mPrice, COL_PRICE);
		REGISTE_PARAM(mStock, COL_STOCK);
	}
};

class SQLiteStdItem : public SQLiteTable
{
public:
	SQLiteStdItem(SQLite* sqlite)
		:SQLiteTable("StdItem", sqlite)
	{
		;
	}
	void query(const string& name, StdItemData& data);
	bool insert(const StdItemData& data);
	bool update(const StdItemData& data);
};

#endif