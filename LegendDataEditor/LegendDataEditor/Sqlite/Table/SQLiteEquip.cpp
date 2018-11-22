#include "SQLiteEquip.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string EquipData::COL_ID = "ID";
string EquipData::COL_LABLE = "Label";
string EquipData::COL_EQUIP_TYPE = "EquipType";
string EquipData::COL_DESC = "Desc";
string EquipData::COL_OCCUPATION = "Occupation";
string EquipData::COL_RESOURCE = "Resource";
string EquipData::COL_CHAR_PANEL_RESOURCE_ATLAS = "CharPanelResourceAtlas";
string EquipData::COL_CHAR_PANEL_RESOURCE_NAME = "CharPanelResourceName";
string EquipData::COL_CHAR_PANEL_RESOURCE_OFFSET = "CharPanelResourceOffset";

void SQLiteEquip::query(int id, EquipData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, EquipData::COL_ID, id, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteEquip::insert(const EquipData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mEquipType);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mOccupation);
	appendValueString(valueString, data.mResource);
	appendValueString(valueString, data.mCharPanelResourceAtlas);
	appendValueString(valueString, data.mCharPanelResourceName);
	appendValueString(valueString, data.mCharPanelResourceOffset, true);
	return doInsert(valueString);
}