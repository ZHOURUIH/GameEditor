#include "SQLiteSceneMapFile.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string SceneMapFileData::COL_ID = "ID";
string SceneMapFileData::COL_FILE_NAME = "FileName";
string SceneMapFileData::COL_MINI_MAP = "MiniMap";
string SceneMapFileData::COL_WIDTH = "Width";
string SceneMapFileData::COL_HEIGHT = "Height";

void SQLiteSceneMapFile::query(int id, SceneMapFileData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, SceneMapFileData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteSceneMapFile::insert(const SceneMapFileData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteSceneMapFile::update(const SceneMapFileData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, SceneMapFileData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}