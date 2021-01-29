#include "SQLiteMonGen.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonGenData::COL_ID = "ID";
string MonGenData::COL_MAP_ID = "MapID";
string MonGenData::COL_POS = "Pos";
string MonGenData::COL_MONSTER_ID = "MonsterID";
string MonGenData::COL_MONSTER_LABEL = "MonsterLabel";
string MonGenData::COL_RADIUS = "Radius";
string MonGenData::COL_COUNT = "Count";
string MonGenData::COL_REFRESH_INTERVAL = "RefreshInterval";

bool SQLiteMonGen::update(const MonGenData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, MonGenData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}