#include "SQLiteSceneMapTransfer.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string SceneMapTransferData::COL_ID = "ID";
string SceneMapTransferData::COL_SOURCE_MAP = "SourceMap";
string SceneMapTransferData::COL_SOURCE_MAP_NAME = "SourceMapName";
string SceneMapTransferData::COL_POSITION = "Position";
string SceneMapTransferData::COL_TARGET_MAP = "TargetMap";
string SceneMapTransferData::COL_TARGET_MAP_NAME = "TargetMapName";
string SceneMapTransferData::COL_TARGET_POSITION = "TargetPosition";

void SQLiteSceneMapTransfer::query(int id, SceneMapTransferData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, SceneMapTransferData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteSceneMapTransfer::insert(const SceneMapTransferData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteSceneMapTransfer::update(const SceneMapTransferData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, SceneMapTransferData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}