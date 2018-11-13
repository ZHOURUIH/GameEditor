#include "SQLiteEquipFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string EquipFrameData::COL_ID = "ID";
string EquipFrameData::COL_LABEL = "Label";
string EquipFrameData::COL_DIRECTION = "Direction";
string EquipFrameData::COL_ACTION = "Action";
string EquipFrameData::COL_FRAME_COUNT = "FrameCount";
string EquipFrameData::COL_POSX = "PosX";
string EquipFrameData::COL_POSY = "PosY";

void SQLiteEquipFrame::query(int id, txVector<EquipFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEquipFrame::query(int id, int direction, txVector<EquipFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, direction, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEquipFrame::query(int id, int direction, const std::string& action, txVector<EquipFrameData>& dataList)
{
	std::string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, EquipFrameData::COL_ACTION, action, "");
	std::string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteEquipFrame::updateData(const EquipFrameData& data)
{
	std::string updateString;
	appendUpdateInt(updateString, EquipFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, EquipFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, EquipFrameData::COL_POSY, data.mPosY, true);
	std::string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, EquipFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}
bool SQLiteEquipFrame::insert(const EquipFrameData& data)
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