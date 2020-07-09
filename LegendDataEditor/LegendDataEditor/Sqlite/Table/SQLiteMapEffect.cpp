#include "SQLiteMapEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MapEffectData::COL_ID = "ID";
string MapEffectData::COL_MAP_FILE_ID = "MapFileID";
string MapEffectData::COL_TILE_POSITION = "TilePosition";
string MapEffectData::COL_EFFECT_FILE_INDEX = "EffectFileIndex";
string MapEffectData::COL_EFFECT_INDEX = "EffectIndex";
string MapEffectData::COL_POSITION_OFFSET = "PositionOffset";
string MapEffectData::COL_SPEED = "Speed";
string MapEffectData::COL_BLEND_TYPE = "BlendType";

void SQLiteMapEffect::query(int id, MapEffectData& data)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MapEffectData::COL_ID, id, "");
	doSelect(data, conditionString);
}

bool SQLiteMapEffect::insert(const MapEffectData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}

bool SQLiteMapEffect::update(const MapEffectData& data)
{
	string updateString;
	data.update(updateString);
	StringUtility::removeLastComma(updateString);
	string conditionStr;
	StringUtility::appendConditionInt(conditionStr, MapEffectData::COL_ID, data.mID, "");
	return doUpdate(updateString, conditionStr);
}