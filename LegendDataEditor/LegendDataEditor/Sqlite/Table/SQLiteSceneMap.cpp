#include "SQLiteSceneMap.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string SceneMapData::COL_ID = "ID";
std::string SceneMapData::COL_LABEL = "Label";
std::string SceneMapData::COL_RESOURCE = "Resource";
std::string SceneMapData::COL_MINI_MAP = "MiniMap";

void SQLiteSceneMap::query(int id, SceneMapData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, SceneMapData::COL_ID, id, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

void SQLiteSceneMap::queryAll(txVector<SceneMapData>& dataList)
{
	std::string queryStr = "SELECT * FROM " + mTableName;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}

bool SQLiteSceneMap::insert(const SceneMapData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}

bool SQLiteSceneMap::update(const SceneMapData& data)
{
	std::string updateStr;
	appendUpdateInt(updateStr, SceneMapData::COL_ID, data.mID);
	appendUpdateString(updateStr, SceneMapData::COL_LABEL, data.mLabel);
	appendUpdateString(updateStr, SceneMapData::COL_RESOURCE, data.mResource);
	appendUpdateInt(updateStr, SceneMapData::COL_MINI_MAP, data.mMiniMap, true);
	std::string conditionStr;
	appendConditionInt(conditionStr, SceneMapData::COL_ID, data.mID, "");
	return doUpdate(updateStr, conditionStr);
}