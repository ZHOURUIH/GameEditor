#include "SQLiteImagePositionEffect.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ImagePositionEffectData::ID = "ID";
string ImagePositionEffectData::PREFAB = "Prefab";
string ImagePositionEffectData::FRAME_COUNT = "FrameCount";
string ImagePositionEffectData::POS_X = "PosX";
string ImagePositionEffectData::POS_Y = "PosY";
string ImagePositionEffectData::SPEED = "Speed";
string ImagePositionEffectData::LOOP = "Loop";

void SQLiteImagePositionEffect::query(const string& prefab, ImagePositionEffectData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ImagePositionEffectData::PREFAB, prefab, "");
	doSelect(data, conditionString);
}