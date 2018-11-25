#include "SQLiteEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string EffectData::COL_LABLE = "Label";
string EffectData::COL_ID = "ID";
string EffectData::COL_DESC = "Desc";
string EffectData::COL_RESOURCE = "Resource";

void SQLiteEffect::query(int weaponID, EffectData& data)
{
	string conditionString;
	appendConditionInt(conditionString, EffectData::COL_ID, weaponID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteEffect::insert(const EffectData& data)
{
	string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mResource);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}