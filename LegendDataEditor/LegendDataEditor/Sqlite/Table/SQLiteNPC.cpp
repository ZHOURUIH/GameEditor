#include "SQLiteNPC.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string NPCData::COL_ID = "ID";
string NPCData::COL_MAP_ID = "MapID";
string NPCData::COL_MAP_NAME = "MapName";
string NPCData::COL_POS = "Pos";
string NPCData::COL_NAME = "Name";
string NPCData::COL_APPERANCE = "Apperance";
string NPCData::COL_STAND_ANIMATION_0 = "StandAnimation0";
string NPCData::COL_STAND_ANIMATION_1 = "StandAnimation1";
string NPCData::COL_GOODS = "Goods";
string NPCData::COL_MAIN_TEXT = "MainText";

void SQLiteNPC::query(txVector<NPCData*>& dataList)
{
	doSelect(dataList);
}
bool SQLiteNPC::insert(const NPCData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}