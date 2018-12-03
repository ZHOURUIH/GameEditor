#ifndef _SQLITE_STD_ITEM_H_
#define _SQLITE_STD_ITEM_H_

#include "SQLiteTable.h"

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
	int mShape;				// 外观
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
	virtual void parse(SQLiteDataReader* reader, SQLiteTable* table)
	{
		mIndex = reader->getInt(table->getCol(COL_INDEX));
		mName = reader->getString(table->getCol(COL_NAME));
		mStdMode = reader->getInt(table->getCol(COL_STD_MODE));
		mShape = reader->getInt(table->getCol(COL_SHAPE));
		mWeight = reader->getInt(table->getCol(COL_WEIGHT));
		mAniCount = reader->getInt(table->getCol(COL_ANI_COUNT));
		mSource = reader->getInt(table->getCol(COL_SOURCE));
		mReserved = reader->getInt(table->getCol(COL_RESERVED));
		mLooks = reader->getInt(table->getCol(COL_LOOKS));
		mDuraMax = reader->getInt(table->getCol(COL_DURA_MAX));
		mMinArmorClass = reader->getInt(table->getCol(COL_MIN_ARMOR_CLASS));
		mMaxArmorClass = reader->getInt(table->getCol(COL_MAX_ARMOR_CLASS));
		mMinMagicArmorClass = reader->getInt(table->getCol(COL_MIN_MAGIC_ARMOR_CLASS));
		mMaxMagicArmorClass = reader->getInt(table->getCol(COL_MAX_MAGIC_ARMOR_CLASS));
		mMinDamageClass = reader->getInt(table->getCol(COL_MIN_DAMAGE_CLASS));
		mMaxDamageClass = reader->getInt(table->getCol(COL_MAX_DAMAGE_CLASS));
		mMinMagicClass = reader->getInt(table->getCol(COL_MIN_MAGIC_CLASS));
		mMaxMagicClass = reader->getInt(table->getCol(COL_MAX_MAGIC_CLASS));
		mMinSC = reader->getInt(table->getCol(COL_MIN_SC));
		mMaxSC = reader->getInt(table->getCol(COL_MAX_SC));
		mNeed = reader->getInt(table->getCol(COL_NEED));
		mNeedLevel = reader->getInt(table->getCol(COL_NEED_LEVEL));
		mPrice = reader->getInt(table->getCol(COL_PRICE));
		mStock = reader->getInt(table->getCol(COL_STOCK));
	}
	virtual void insert(string& valueString) const
	{
		StringUtility::appendValueInt(valueString, mIndex);
		StringUtility::appendValueString(valueString, mName);
		StringUtility::appendValueInt(valueString, mStdMode);
		StringUtility::appendValueInt(valueString, mShape);
		StringUtility::appendValueInt(valueString, mWeight);
		StringUtility::appendValueInt(valueString, mAniCount);
		StringUtility::appendValueInt(valueString, mSource);
		StringUtility::appendValueInt(valueString, mReserved);
		StringUtility::appendValueInt(valueString, mLooks);
		StringUtility::appendValueInt(valueString, mDuraMax);
		StringUtility::appendValueInt(valueString, mMinArmorClass);
		StringUtility::appendValueInt(valueString, mMaxArmorClass);
		StringUtility::appendValueInt(valueString, mMinMagicArmorClass);
		StringUtility::appendValueInt(valueString, mMaxMagicArmorClass);
		StringUtility::appendValueInt(valueString, mMinDamageClass);
		StringUtility::appendValueInt(valueString, mMaxDamageClass);
		StringUtility::appendValueInt(valueString, mMinMagicClass);
		StringUtility::appendValueInt(valueString, mMaxMagicClass);
		StringUtility::appendValueInt(valueString, mMinSC);
		StringUtility::appendValueInt(valueString, mMaxSC);
		StringUtility::appendValueInt(valueString, mNeed);
		StringUtility::appendValueInt(valueString, mNeedLevel);
		StringUtility::appendValueInt(valueString, mPrice);
		StringUtility::appendValueInt(valueString, mStock);
	}
	virtual void update(string& updateString) const
	{
		StringUtility::appendUpdateInt(updateString, COL_INDEX, mIndex);
		StringUtility::appendUpdateString(updateString, COL_NAME, mName);
		StringUtility::appendUpdateInt(updateString, COL_STD_MODE, mStdMode);
		StringUtility::appendUpdateInt(updateString, COL_SHAPE, mShape);
		StringUtility::appendUpdateInt(updateString, COL_WEIGHT, mWeight);
		StringUtility::appendUpdateInt(updateString, COL_ANI_COUNT, mAniCount);
		StringUtility::appendUpdateInt(updateString, COL_SOURCE, mSource);
		StringUtility::appendUpdateInt(updateString, COL_RESERVED, mReserved);
		StringUtility::appendUpdateInt(updateString, COL_LOOKS, mLooks);
		StringUtility::appendUpdateInt(updateString, COL_DURA_MAX, mDuraMax);
		StringUtility::appendUpdateInt(updateString, COL_MIN_ARMOR_CLASS, mMinArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_MAX_ARMOR_CLASS, mMaxArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_MIN_MAGIC_ARMOR_CLASS, mMinMagicArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_MAX_MAGIC_ARMOR_CLASS, mMaxMagicArmorClass);
		StringUtility::appendUpdateInt(updateString, COL_MIN_DAMAGE_CLASS, mMinDamageClass);
		StringUtility::appendUpdateInt(updateString, COL_MAX_DAMAGE_CLASS, mMaxDamageClass);
		StringUtility::appendUpdateInt(updateString, COL_MIN_MAGIC_CLASS, mMinMagicClass);
		StringUtility::appendUpdateInt(updateString, COL_MAX_MAGIC_CLASS, mMaxMagicClass);
		StringUtility::appendUpdateInt(updateString, COL_MIN_SC, mMinSC);
		StringUtility::appendUpdateInt(updateString, COL_MAX_SC, mMaxSC);
		StringUtility::appendUpdateInt(updateString, COL_NEED, mNeed);
		StringUtility::appendUpdateInt(updateString, COL_NEED_LEVEL, mNeedLevel);
		StringUtility::appendUpdateInt(updateString, COL_PRICE, mPrice);
		StringUtility::appendUpdateInt(updateString, COL_STOCK, mStock);
	}
	static void registeColumn(SQLiteTable* table)
	{
		table->registeColumn(COL_INDEX);
		table->registeColumn(COL_NAME);
		table->registeColumn(COL_STD_MODE);
		table->registeColumn(COL_SHAPE);
		table->registeColumn(COL_WEIGHT);
		table->registeColumn(COL_ANI_COUNT);
		table->registeColumn(COL_SOURCE);
		table->registeColumn(COL_RESERVED);
		table->registeColumn(COL_LOOKS);
		table->registeColumn(COL_DURA_MAX);
		table->registeColumn(COL_MIN_ARMOR_CLASS);
		table->registeColumn(COL_MAX_ARMOR_CLASS);
		table->registeColumn(COL_MIN_MAGIC_ARMOR_CLASS);
		table->registeColumn(COL_MAX_MAGIC_ARMOR_CLASS);
		table->registeColumn(COL_MIN_DAMAGE_CLASS);
		table->registeColumn(COL_MAX_DAMAGE_CLASS);
		table->registeColumn(COL_MIN_MAGIC_CLASS);
		table->registeColumn(COL_MAX_MAGIC_CLASS);
		table->registeColumn(COL_MIN_SC);
		table->registeColumn(COL_MAX_SC);
		table->registeColumn(COL_NEED);
		table->registeColumn(COL_NEED_LEVEL);
		table->registeColumn(COL_PRICE);
		table->registeColumn(COL_STOCK);
	}
};

class SQLiteStdItem : public SQLiteTable
{
public:
	SQLiteStdItem(SQLite* sqlite)
		:SQLiteTable("StdItem", sqlite)
	{
		StdItemData::registeColumn(this);
	}
	void query(const string& name, StdItemData& data);
	void queryAll(txVector<StdItemData>& dataList);
	bool insert(const StdItemData& data);
	bool update(const StdItemData& data);
};

#endif