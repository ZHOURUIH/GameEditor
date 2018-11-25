#include "SQLiteMonster.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterData::COL_LABLE = "MonsterLabel";
string MonsterData::COL_ID = "MonsterID";
string MonsterData::COL_DESC = "Desc";
string MonsterData::COL_PREFAB = "Prefab";
string MonsterData::COL_RESOURCE = "Resource";

void SQLiteMonster::query(int monsterID, MonsterData& data)
{
	string conditionString;
	appendConditionInt(conditionString, MonsterData::COL_ID, monsterID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteMonster::insert(const MonsterData& data)
{
	string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mPrefab);
	appendValueString(valueString, data.mResource);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}