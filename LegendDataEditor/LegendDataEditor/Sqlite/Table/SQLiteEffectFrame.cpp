#include "SQLiteEffectFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string EffectFrameData::COL_ID = "ID";
string EffectFrameData::COL_LABEL = "Label";
string EffectFrameData::COL_DIRECTION = "Direction";
string EffectFrameData::COL_ACTION = "Action";
string EffectFrameData::COL_FRAME_COUNT = "FrameCount";
string EffectFrameData::COL_POSX = "PosX";
string EffectFrameData::COL_POSY = "PosY";

void SQLiteEffectFrame::query(int ID, txVector<EffectFrameData>& dataList)
{
	string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, txVector<EffectFrameData>& dataList)
{
	string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, const string& action, txVector<EffectFrameData>& dataList)
{
	string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	appendConditionString(conditionString, EffectFrameData::COL_ACTION, action, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteEffectFrame::updateData(const EffectFrameData& data)
{
	string updateString;
	appendUpdateInt(updateString, EffectFrameData::COL_FRAME_COUNT, data.mFrameCount);
	appendUpdateIntArray(updateString, EffectFrameData::COL_POSX, data.mPosX);
	appendUpdateIntArray(updateString, EffectFrameData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	appendConditionInt(conditionString, EffectFrameData::COL_ID, data.mID, " and ");
	appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, data.mDirection, " and ");
	appendConditionString(conditionString, EffectFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteEffectFrame::insert(const EffectFrameData& data)
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