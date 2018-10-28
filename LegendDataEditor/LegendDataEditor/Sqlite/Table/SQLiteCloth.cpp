#include "SQLiteCloth.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string ClothData::COL_LABLE = "ClothLabel";
std::string ClothData::COL_ID = "ClothID";
std::string ClothData::COL_DESC = "Desc";
std::string ClothData::COL_OCCUPATION = "Occupation";
std::string ClothData::COL_RESOURCE = "Resource";

void SQLiteCloth::query(int clothID, ClothData& data)
{
	std::string conditionString;
	appendConditionInt(conditionString, ClothData::COL_ID, clothID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), data);
}

bool SQLiteCloth::insert(const ClothData& data)
{
	std::string valueString;
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mDesc);
	appendValueString(valueString, data.mOccupation);
	appendValueString(valueString, data.mResource, true);
	return doInsert(valueString);
}