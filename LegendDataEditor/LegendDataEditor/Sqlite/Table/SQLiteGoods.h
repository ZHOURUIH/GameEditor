#ifndef _SQLITE_GOODS_H_
#define _SQLITE_GOODS_H_

#include "SQLiteTable.h"
#include "SQLiteTableData.h"

class GoodsData : public SQLiteTableData
{
public:
	static string COL_ID;
	static string COL_NPC;
	static string COL_NAME;
	static string COL_ITEM;
	static string COL_PRICE_GOLD;
public:
	int mID;
	int mNPC;
	string mName;
	int mItem;
	int mPriceGold;
public:
	GoodsData()
	{
		REGISTE_PARAM(mID, COL_ID);
		REGISTE_PARAM(mNPC, COL_NPC);
		REGISTE_PARAM(mName, COL_NAME);
		REGISTE_PARAM(mItem, COL_ITEM);
		REGISTE_PARAM(mPriceGold, COL_PRICE_GOLD);
	}
};

class SQLiteGoods : public SQLiteTable
{
public:
	SQLiteGoods(SQLite* sqlite)
		:SQLiteTable("Goods", sqlite){}
	bool update(const GoodsData& data);
};

#endif