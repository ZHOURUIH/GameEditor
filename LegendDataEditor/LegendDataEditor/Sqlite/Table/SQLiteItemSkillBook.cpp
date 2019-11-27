#include "SQLiteItemSkillBook.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ItemSkillBookData::COL_ID = "ID";
string ItemSkillBookData::COL_NAME = "Name";
string ItemSkillBookData::COL_DESCRIPTION = "Description";
string ItemSkillBookData::COL_SKILL_ID = "SkillID";
string ItemSkillBookData::COL_JOB = "Job";
string ItemSkillBookData::COL_LEVEL = "Level";
string ItemSkillBookData::COL_ICON_ATLAS = "IconAtlas";
string ItemSkillBookData::COL_ICON = "Icon";
string ItemSkillBookData::COL_PRICE = "Price";
string ItemSkillBookData::COL_WEIGHT = "Weight";

bool SQLiteItemSkillBook::query(int id, ItemSkillBookData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, ItemSkillBookData::COL_ID, id, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemSkillBook::query(const string& name, ItemSkillBookData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ItemSkillBookData::COL_NAME, name, "");
	return doSelect(data, conditionString);
}

bool SQLiteItemSkillBook::insert(const ItemSkillBookData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteItemSkillBook::update(const ItemSkillBookData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, ItemSkillBookData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}