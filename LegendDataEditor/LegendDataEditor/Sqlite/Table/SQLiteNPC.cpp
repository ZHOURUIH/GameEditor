#include "SQLiteNPC.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string NPCData::COL_ID = "ID";
string NPCData::COL_SCRIPT = "Script";
string NPCData::COL_MAP_ID = "MapID";
string NPCData::COL_MAP_NAME = "MapName";
string NPCData::COL_POSX = "PosX";
string NPCData::COL_POSY = "PosY";
string NPCData::COL_LABEL = "Label";
string NPCData::COL_FLAG = "Flag";
string NPCData::COL_APPERANCE = "Apperance";

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