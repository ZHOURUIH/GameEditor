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
	string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEquipFrame::query(int id, int direction, txVector<EquipFrameData>& dataList)
{
	string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEquipFrame::query(int id, int direction, const string& action, txVector<EquipFrameData>& dataList)
{
	string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, id, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, direction, " and ");
	appendConditionString(conditionString, EquipFrameData::COL_ACTION, action, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteEquipFrame::updateData(const EquipFrameData& data)
{
	string updateString;
	appendUpdateInt(updateString, EquipFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, EquipFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, EquipFrameData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	appendConditionInt(conditionString, EquipFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, EquipFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, EquipFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}
bool SQLiteEquipFrame::insert(const EquipFrameData& data)
{
	string valueString;
	appendValueInt(valueString, data.mID);
	appendValueString(valueString, data.mLabel);
	appendValueInt(valueString, data.mDirection);
	appendValueString(valueString, data.mAction);
	appendValueInt(valueString, data.mFrameCount);
	appendValueIntArray(valueString, data.mPosX);
	appendValueIntArray(valueString, data.mPosY);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}