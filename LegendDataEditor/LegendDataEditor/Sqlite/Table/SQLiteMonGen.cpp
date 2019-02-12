#include "SQLiteMonGen.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonGenData::COL_MAP= "Map";
string MonGenData::COL_POSX = "PosX";
string MonGenData::COL_POSY = "PosY";
string MonGenData::COL_ID = "ID";
string MonGenData::COL_MONSTER_LABEL = "MonsterLabel";
string MonGenData::COL_RADIUS = "Radius";
string MonGenData::COL_COUNT = "Count";
string MonGenData::COL_MINUTE_INTERVAL = "MinuteInterval";

bool SQLiteMonGen::query(const MonGenData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonGenData::COL_MAP, data.mMap, " and ");
	StringUtility::appendConditionInt(conditionString, MonGenData::COL_POSX, data.mPosX, " and ");
	StringUtility::appendConditionInt(conditionString, MonGenData::COL_POSY, data.mPosY, " and ");
	StringUtility::appendConditionString(conditionString, MonGenData::COL_MONSTER_LABEL, data.mMonsterLabel, " and ");
	StringUtility::appendConditionInt(conditionString, MonGenData::COL_RADIUS, data.mRadius, " and ");
	StringUtility::appendConditionInt(conditionString, MonGenData::COL_COUNT, data.mCount, "");
	MonGenData tempData;
	return doSelect(tempData, conditionString);
}

void SQLiteMonGen::query(string map, txVector<MonGenData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, MonGenData::COL_MAP, map, "");
	doSelect(dataList, conditionString);
}

bool SQLiteMonGen::insert(const MonGenData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}