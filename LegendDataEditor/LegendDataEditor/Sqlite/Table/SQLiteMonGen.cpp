#include "SQLiteMonGen.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonGenData::COL_MAP= "Map";
string MonGenData::COL_POSX = "PosX";
string MonGenData::COL_POSY = "PosY";
string MonGenData::COL_MONSTER_LABEL = "MonsterLabel";
string MonGenData::COL_RADIUS = "Radius";
string MonGenData::COL_COUNT = "Count";
string MonGenData::COL_MINUTE_INTERVAL = "MinuteInterval";

void SQLiteMonGen::query(string map, MonGenData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonGenData::COL_MAP, map, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

void SQLiteMonGen::queryAll(txVector<MonGenData>& dataList)
{
	string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}

bool SQLiteMonGen::insert(const MonGenData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}