#include "SQLiteMonster.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string MonsterData::COL_LABLE = "MonsterLabel";
std::string MonsterData::COL_ID = "MonsterID";
std::string MonsterData::COL_DESC = "Desc";
std::string MonsterData::COL_PREFAB = "Prefab";
std::string MonsterData::COL_RESOURCE = "Resource";

void SQLiteMonster::query(int monsterID, MonsterData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, MonsterData::COL_ID, monsterID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteMonster::insert(const MonsterData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mPrefab);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}