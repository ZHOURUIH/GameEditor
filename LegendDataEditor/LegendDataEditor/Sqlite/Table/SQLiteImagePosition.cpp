#include "SQLiteImagePosition.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string ImagePositionData::COL_ATALS = "Atlas";
string ImagePositionData::COL_IMAGE = "Image";
string ImagePositionData::COL_POS_X = "PosX";
string ImagePositionData::COL_POS_Y = "PosY";

void SQLiteImagePosition::query(const string& atlas, const string& image, ImagePositionData& data)
{
	string conditionString;
	StringUtility::appendConditionString(conditionString, ImagePositionData::COL_ATALS, atlas, " and ");
	StringUtility::appendConditionString(conditionString, ImagePositionData::COL_IMAGE, image, "");
	doSelect(data, conditionString);
}

bool SQLiteImagePosition::insert(const ImagePositionData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}