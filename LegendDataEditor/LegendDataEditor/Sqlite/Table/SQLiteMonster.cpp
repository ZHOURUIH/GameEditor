#include "SQLiteMonster.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterData::COL_LABEL = "Label";
string MonsterData::COL_ID = "ID";
string MonsterData::COL_DESC = "Desc";
string MonsterData::COL_PREFAB = "Prefab";
string MonsterData::COL_RESOURCE = "Resource";

void SQLiteMonster::query(int monsterID, MonsterData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterData::COL_ID, monsterID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteMonster::insert(const MonsterData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}