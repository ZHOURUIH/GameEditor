#include "SQLiteMonsterTemplate.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterTemplateData::COL_ID = "ID";
string MonsterTemplateData::COL_NAME = "Name";
string MonsterTemplateData::COL_RACE = "Race";
string MonsterTemplateData::COL_RACE_IMAGE = "RaceImage";
string MonsterTemplateData::COL_MODEL = "Model";
string MonsterTemplateData::COL_APPERANCE = "Apperance";
string MonsterTemplateData::COL_MODEL_RADIUS = "ModelRadius";
string MonsterTemplateData::COL_MONSTER_RACE = "MonsterRace";
string MonsterTemplateData::COL_COOL_EYE = "CoolEye";

void SQLiteMonsterTemplate::query(int id, MonsterTemplateData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterTemplateData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteMonsterTemplate::insert(const MonsterTemplateData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteMonsterTemplate::update(const MonsterTemplateData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionString(conditionStr, MonsterTemplateData::COL_ID, data.mName, "");
	return doUpdate(updateString, conditionStr);
}