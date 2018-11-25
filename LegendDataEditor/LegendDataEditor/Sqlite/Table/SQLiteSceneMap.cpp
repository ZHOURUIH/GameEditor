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
	appendConditionInt(conditionString, SceneMapData::COL_ID, id, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

void SQLiteSceneMap::queryAll(txVector<SceneMapData>& dataList)
{
	string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}

bool SQLiteSceneMap::insert(const SceneMapData& data)
{
	string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueString(valueString, data.mResource);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteSceneMap::update(const SceneMapData& data)
{
	string updateString;
	appendUpdateInt(updateString, SceneMapData::COL_ID, data.mID);
	appendUpdateString(updateString, SceneMapData::COL_LABEL, data.mLabel);
	appendUpdateString(updateString, SceneMapData::COL_RESOURCE, data.mResource);
	appendUpdateInt(updateString, SceneMapData::COL_MINI_MAP, data.mMiniMap);
	appendUpdateInt(updateString, SceneMapData::COL_WIDTH, data.mWidth);
	appendUpdateInt(updateString, SceneMapData::COL_HEIGHT, data.mHeight);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	appendConditionInt(conditionStr, SceneMapData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}