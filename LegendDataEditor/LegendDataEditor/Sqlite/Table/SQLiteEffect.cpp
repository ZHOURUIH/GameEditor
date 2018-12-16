#include "SQLiteEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string EffectData::COL_LABEL = "Label";
string EffectData::COL_ID = "ID";
string EffectData::COL_DESC = "Desc";
string EffectData::COL_RESOURCE = "Resource";

void SQLiteEffect::query(int weaponID, EffectData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, EffectData::COL_ID, weaponID, "");
	doSelect(data, conditionString);
}

bool SQLiteEffect::insert(const EffectData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}