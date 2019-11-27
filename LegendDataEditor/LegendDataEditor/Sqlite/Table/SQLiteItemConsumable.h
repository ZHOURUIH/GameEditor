#ifndef _SQLITE_ITEM_CONSUMABLE_H_
#define _SQLITE_ITEM_CONSUMABLE_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class ItemConsumableData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NAME;
	static string COL_DESCRIPTION;
	static string COL_LEVEL;
	static string COL_STD_MODE;
	static string COL_SHAPE;
	static string COL_BUFF;
	static string COL_BUFF_PARAM0;
	static string COL_BUFF_PARAM1;
	static string COL_BUFF_PARAM2;
	static string COL_BUFF_TIME;
	static string COL_MAX_USE_COUNT;
	static string COL_WEIGHT;
	static string COL_ANI_COUNT;
	static string COL_LOOKS;
	static string COL_PRICE;
public:
	int mID;
	string mName;
	string mDescription;
	int mLevel;
	int mStdMode;
	int mShape;
	string mBuff;
	string mBuffParam0;
	string mBuffParam1;
	string mBuffParam2;
	string mBuffTime;
	int mMaxUseCount;
	int mWeight;
	int mAniCount;
	int mLooks;
	int mPrice;
public:
	ItemConsumableData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mDescription, COL_DESCRIPTION);
		REGISTE_PARAM(mLevel, COL_LEVEL);
		REGISTE_PARAM(mStdMode, COL_STD_MODE);
		REGISTE_PARAM(mShape, COL_SHAPE);
		REGISTE_PARAM(mBuff, COL_BUFF);
		REGISTE_PARAM(mBuffParam0, COL_BUFF_PARAM0);
		REGISTE_PARAM(mBuffParam1, COL_BUFF_PARAM1);
		REGISTE_PARAM(mBuffParam2, COL_BUFF_PARAM2);
		REGISTE_PARAM(mBuffTime, COL_BUFF_TIME);
		REGISTE_PARAM(mMaxUseCount, COL_MAX_USE_COUNT);
		REGISTE_PARAM(mWeight, COL_WEIGHT);
		REGISTE_PARAM(mAniCount, COL_ANI_COUNT);
		REGISTE_PARAM(mLooks, COL_LOOKS);
		REGISTE_PARAM(mPrice, COL_PRICE);
	}
};

class SQLiteItemConsumable : public SQLiteTable
{
public:
	SQLiteItemConsumable(SQLite* sqlite)
		:SQLiteTable("ItemConsumable", sqlite)
	{}
	bool query(int id, ItemConsumableData& data);
	bool query(const string& name, ItemConsumableData& data);
	bool insert(const ItemConsumableData& data);
	bool update(const ItemConsumableData& data);
};

#endif