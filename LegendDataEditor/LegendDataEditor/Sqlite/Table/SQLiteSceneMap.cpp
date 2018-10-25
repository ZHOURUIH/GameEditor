#include "SQLiteSceneMap.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

void SQLiteSceneMap::query(int id, SceneMapData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, COL_ID, id, "");
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
	appendUpdateInt(updateStr, COL_ID, data.mID);
	appendUpdateString(updateStr, COL_LABEL, data.mLabel);
	appendUpdateString(updateStr, COL_RESOURCE, data.mResource, true);
	std::string conditionStr;
	appendConditionInt(conditionStr, COL_ID, data.mID, "");
	return doUpdate(updateStr, conditionStr);
}

void SQLiteSceneMap::parseReader(SQLiteDataReader* reader, SceneMapData& data)
{
	while (reader->read())
	{
		data.mID = reader->getInt(getCol(COL_ID));
		data.mLabel = reader->getString(getCol(COL_LABEL));
		data.mResource = reader->getString(getCol(COL_RESOURCE));
		break;
	}
	mSQLite->releaseReader(reader);
}

void SQLiteSceneMap::parseReader(SQLiteDataReader* reader, txVector<SceneMapData>& dataList)
{
	while (reader->read())
	{
		SceneMapData data;
		data.mID = reader->getInt(getCol(COL_ID));
		data.mLabel = reader->getString(getCol(COL_LABEL));
		data.mResource = reader->getString(getCol(COL_RESOURCE));
		dataList.push_back(data);
	}
	mSQLite->releaseReader(reader);
}