#include "SQLiteClothFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string ClothFrameData::COL_ID = "ClothID";
std::string ClothFrameData::COL_LABEL = "Label";
std::string ClothFrameData::COL_DIRECTION = "Direction";
std::string ClothFrameData::COL_ACTION = "Action";
std::string ClothFrameData::COL_FRAME_COUNT = "FrameCount";
std::string ClothFrameData::COL_POSX = "PosX";
std::string ClothFrameData::COL_POSY = "PosY";

void SQLiteClothFrame::query(int clothID, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, ClothFrameData::COL_ID, clothID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteClothFrame::query(int clothID, int direction, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, ClothFrameData::COL_ID, clothID, " and ");
	appendConditionInt(conditionString, ClothFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteClothFrame::query(int clothID, int direction, const std::string& action, txVector<ClothFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, ClothFrameData::COL_ID, clothID, " and ");
	appendConditionInt(conditionString, ClothFrameData::COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, ClothFrameData::COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteClothFrame::updateData(const ClothFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, ClothFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, ClothFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, ClothFrameData::COL_POSY, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, ClothFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, ClothFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, ClothFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}
bool SQLiteClothFrame::insert(const ClothFrameData& data)
{
	std::string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mDirection);
	appendValueString(valueString, data.mAction);
	appendValueInt(valueString, data.mFrameCount);
	appendValueIntArray(valueString, data.mPosX);
	appendValueIntArray(valueString, data.mPosY, true);
	return doInsert(valueString);
}