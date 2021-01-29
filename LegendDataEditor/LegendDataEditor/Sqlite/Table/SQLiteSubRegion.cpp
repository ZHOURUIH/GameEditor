#include "SQLiteSubRegion.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string SubRegionData::COL_ID = "ID";
string SubRegionData::COL_NAME = "Name";
string SubRegionData::COL_MAP_ID = "MapID";
string SubRegionData::COL_POSITION = "Position";
string SubRegionData::COL_SIZE = "Size";

bool SQLiteSubRegion::update(const SubRegionData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, SubRegionData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}