#include "SQLiteItemConsumable.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ItemConsumableData::COL_ID = "ID";
string ItemConsumableData::COL_NAME = "Name";
string ItemConsumableData::COL_DESCRIPTION = "Description";
string ItemConsumableData::COL_LEVEL = "Level";
string ItemConsumableData::COL_STD_MODE = "StdMode";
string ItemConsumableData::COL_SHAPE = "Shape";
string ItemConsumableData::COL_BUFF = "Buff";
string ItemConsumableData::COL_BUFF_PARAM0 = "BuffParam0";
string ItemConsumableData::COL_BUFF_PARAM1 = "BuffParam1";
string ItemConsumableData::COL_BUFF_PARAM2 = "BuffParam2";
string ItemConsumableData::COL_BUFF_TIME = "BuffTime";
string ItemConsumableData::COL_MAX_USE_COUNT = "MaxUseCount";
string ItemConsumableData::COL_WEIGHT = "Weight";
string ItemConsumableData::COL_ANI_COUNT = "AniCount";
string ItemConsumableData::COL_LOOKS = "Looks";
string ItemConsumableData::COL_PRICE = "Price";

bool SQLiteItemConsumable::query(int id, ItemConsumableData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, ItemConsumableData::COL_ID, id, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemConsumable::query(const string& name, ItemConsumableData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ItemConsumableData::COL_NAME, name, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemConsumable::insert(const ItemConsumableData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteItemConsumable::update(const ItemConsumableData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, ItemConsumableData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}