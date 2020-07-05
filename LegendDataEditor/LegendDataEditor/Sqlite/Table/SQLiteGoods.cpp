#include "SQLiteGoods.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string GoodsData::COL_ID = "ID";
string GoodsData::COL_NPC = "NPC";
string GoodsData::COL_NAME = "Name";
string GoodsData::COL_ITEM = "Item";
string GoodsData::COL_PRICE_GOLD = "PriceGold";

bool SQLiteGoods::update(const GoodsData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, GoodsData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}