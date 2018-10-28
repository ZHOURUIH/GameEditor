#include "SQLiteEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string EffectData::COL_LABLE = "Label";
std::string EffectData::COL_ID = "ID";
std::string EffectData::COL_DESC = "Desc";
std::string EffectData::COL_RESOURCE = "Resource";

void SQLiteEffect::query(int weaponID, EffectData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, EffectData::COL_ID, weaponID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteEffect::insert(const EffectData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}