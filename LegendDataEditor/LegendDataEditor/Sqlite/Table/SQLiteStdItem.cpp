#include "SQLiteStdItem.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string StdItemData::COL_INDEX = "Index";
string StdItemData::COL_NAME = "Name";
string StdItemData::COL_STD_MODE = "StdMode";
string StdItemData::COL_SHAPE = "Shape";
string StdItemData::COL_WEIGHT = "Weight";
string StdItemData::COL_ANI_COUNT = "AniCount";
string StdItemData::COL_SOURCE = "Source";
string StdItemData::COL_RESERVED = "Reserved";
string StdItemData::COL_LOOKS = "Looks";
string StdItemData::COL_DURA_MAX = "DuraMax";
string StdItemData::COL_MIN_ARMOR_CLASS = "MinArmorClass";
string StdItemData::COL_MAX_ARMOR_CLASS = "MaxArmorClass";
string StdItemData::COL_MIN_MAGIC_ARMOR_CLASS = "MinMagicArmorClass";
string StdItemData::COL_MAX_MAGIC_ARMOR_CLASS = "MaxMagicArmorClass";
string StdItemData::COL_MIN_DAMAGE_CLASS = "MinDamageClass";
string StdItemData::COL_MAX_DAMAGE_CLASS = "MaxDamageClass";
string StdItemData::COL_MIN_MAGIC_CLASS = "MinMagicClass";
string StdItemData::COL_MAX_MAGIC_CLASS = "MaxMagicClass";
string StdItemData::COL_MIN_SC = "MinSC";
string StdItemData::COL_MAX_SC = "MaxSC";
string StdItemData::COL_NEED = "Need";
string StdItemData::COL_NEED_LEVEL = "NeedLevel";
string StdItemData::COL_PRICE = "Price";
string StdItemData::COL_STOCK = "Stock";

void SQLiteStdItem::query(const string& name, StdItemData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, StdItemData::COL_NAME, name, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

void SQLiteStdItem::queryAll(txVector<StdItemData>& dataList)
{
	string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}

bool SQLiteStdItem::insert(const StdItemData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteStdItem::update(const StdItemData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionString(conditionStr, StdItemData::COL_NAME, data.mName, "");
	return doUpdate(updateString, conditionStr);
}