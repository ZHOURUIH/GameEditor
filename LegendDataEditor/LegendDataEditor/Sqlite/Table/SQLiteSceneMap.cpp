#include "SQLiteSceneMap.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string SceneMapData::COL_ID = "ID";
string SceneMapData::COL_LABEL = "Label";
string SceneMapData::COL_RESOURCE = "Resource";
string SceneMapData::COL_MINI_MAP = "MiniMap";
string SceneMapData::COL_WIDTH = "Width";
string SceneMapData::COL_HEIGHT = "Height";

void SQLiteSceneMap::query(int id, SceneMapData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, SceneMapData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteSceneMap::insert(const SceneMapData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteSceneMap::update(const SceneMapData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, SceneMapData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}