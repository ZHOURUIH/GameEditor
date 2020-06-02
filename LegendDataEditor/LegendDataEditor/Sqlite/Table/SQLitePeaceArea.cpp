#include "SQLitePeaceArea.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string PeaceAreaData::COL_ID = "ID";
string PeaceAreaData::COL_MAP_ID = "MapID";
string PeaceAreaData::COL_MAP_NAME = "MapName";
string PeaceAreaData::COL_POSITION = "Position";
string PeaceAreaData::COL_SIZE = "Size";

bool SQLitePeaceArea::update(const PeaceAreaData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, PeaceAreaData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}