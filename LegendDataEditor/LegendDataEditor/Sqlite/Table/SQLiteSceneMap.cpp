#include "SQLiteSceneMap.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string SceneMapData::COL_ID = "ID";
string SceneMapData::COL_LABEL = "Label";
string SceneMapData::COL_MAP_FILE = "MapFile";
string SceneMapData::COL_MONSTER_REGION = "MonsterRegion";
string SceneMapData::COL_NPC = "NPC";
string SceneMapData::COL_TRANSFER_POINT = "TransferPoint";
string SceneMapData::COL_PEACE_AREA = "PeaceArea";
string SceneMapData::COL_MAIN_CITY = "MainCity";
string SceneMapData::COL_SUB_REGION = "SubRegion";

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