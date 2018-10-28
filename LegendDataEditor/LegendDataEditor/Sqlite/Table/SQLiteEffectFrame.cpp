#include "SQLiteEffectFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

std::string EffectFrameData::COL_ID = "ID";
std::string EffectFrameData::COL_LABEL = "Label";
std::string EffectFrameData::COL_DIRECTION = "Direction";
std::string EffectFrameData::COL_ACTION = "Action";
std::string EffectFrameData::COL_FRAME_COUNT = "FrameCount";
std::string EffectFrameData::COL_POSX = "PosX";
std::string EffectFrameData::COL_POSY = "PosY";

void SQLiteEffectFrame::query(int ID, txVector<EffectFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, txVector<EffectFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, const std::string& action, txVector<EffectFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	appendConditionString(conditionString, EffectFrameData::COL_ACTION, action, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteEffectFrame::updateData(const EffectFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, EffectFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, EffectFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, EffectFrameData::COL_POSY, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, EffectFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteEffectFrame::insert(const EffectFrameData& data)
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