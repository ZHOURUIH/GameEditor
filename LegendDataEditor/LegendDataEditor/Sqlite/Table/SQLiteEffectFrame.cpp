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
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, txVector<EffectFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteEffectFrame::query(int ID, int direction, const string& action, txVector<EffectFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_ID, ID, " and ");
	StringUtility::appendConditionString(conditionString, EffectFrameData::COL_ACTION, action, " and ");
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteEffectFrame::updateData(const EffectFrameData& data)
{
	string updateString;
	StringUtility::appendUpdateInt(updateString, EffectFrameData::COL_FRAME_COUNT, data.mFrameCount);
	StringUtility::appendUpdateIntArray(updateString, EffectFrameData::COL_POSX, data.mPosX);
	StringUtility::appendUpdateIntArray(updateString, EffectFrameData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_ID, data.mID, " and ");
	StringUtility::appendConditionInt(conditionString, EffectFrameData::COL_DIRECTION, data.mDirection, " and ");
	StringUtility::appendConditionString(conditionString, EffectFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteEffectFrame::insert(const EffectFrameData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}