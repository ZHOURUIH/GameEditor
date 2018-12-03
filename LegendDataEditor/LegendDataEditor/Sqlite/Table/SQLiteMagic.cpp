#include "SQLiteMagic.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MagicData::COL_ID = "ID";
string MagicData::COL_NAME = "Name";
string MagicData::COL_EFFECT_TYPE = "EffectType";
string MagicData::COL_EFFECT = "Effect";
string MagicData::COL_SPELL = "Spell";
string MagicData::COL_POWER = "Power";
string MagicData::COL_MAX_POWER = "MaxPower";
string MagicData::COL_DEF_SPELL = "DefSpell";
string MagicData::COL_DEF_POWER = "DefPower";
string MagicData::COL_DEF_MAX_POWER = "DefMaxPower";
string MagicData::COL_JOB = "Job";
string MagicData::COL_NEED_LEVEL_1 = "NeedLevel1";
string MagicData::COL_LEVEL_TRAIN_1 = "LevelTrain1";
string MagicData::COL_NEED_LEVEL_2 = "NeedLevel2";
string MagicData::COL_LEVEL_TRAIN_2 = "LevelTrain2";
string MagicData::COL_NEED_LEVEL_3 = "NeedLevel3";
string MagicData::COL_LEVEL_TRAIN_3 = "LevelTrain3";
string MagicData::COL_DELAY = "Delay";
string MagicData::COL_DESCRIBE = "Describe";

void SQLiteMagic::query(const string& name, MagicData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MagicData::COL_NAME, name, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

void SQLiteMagic::queryAll(txVector<MagicData>& dataList)
{
	string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}

bool SQLiteMagic::insert(const MagicData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteMagic::update(const MagicData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionString(conditionStr, MagicData::COL_NAME, data.mName, "");
	return doUpdate(updateString, conditionStr);
}