#include "SQLiteMonsterFrame.h"
#include "Utility.h"
#include "SQLiteDataReader.h"
#include "SQLite.h"

string MonsterFrameData::COL_ID = "MonsterID";
string MonsterFrameData::COL_LABEL = "Label";
string MonsterFrameData::COL_DIRECTION = "Direction";
string MonsterFrameData::COL_ACTION = "Action";
string MonsterFrameData::COL_FRAME_COUNT = "FrameCount";
string MonsterFrameData::COL_POSX = "PosX";
string MonsterFrameData::COL_POSY = "PosY";

void SQLiteMonsterFrame::query(int monsterID, txVector<MonsterFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, txVector<MonsterFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, " and ");
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, direction, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
void SQLiteMonsterFrame::query(int monsterID, int direction, const string& action, txVector<MonsterFrameData>& dataList)
{
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_ID, monsterID, " and ");
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, direction, " and ");
	StringUtility::appendConditionString(conditionString, MonsterFrameData::COL_ACTION, action, "");
	string queryStr = "SELECT * FROM " + mTableName + " WHERE " + conditionString;
	parseReader(mSQLite->executeQuery(queryStr), dataList);
}
bool SQLiteMonsterFrame::updateData(const MonsterFrameData& data)
{
	string updateString;
	StringUtility::appendUpdateInt(updateString, MonsterFrameData::COL_FRAME_COUNT, data.mFrameCount);
	StringUtility::appendUpdateIntArray(updateString, MonsterFrameData::COL_POSX, data.mPosX);
	StringUtility::appendUpdateIntArray(updateString, MonsterFrameData::COL_POSY, data.mPosY);
	StringUtility::removeLastComma(updateString);
	string conditionString;
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_ID, data.mID, " and ");
	StringUtility::appendConditionInt(conditionString, MonsterFrameData::COL_DIRECTION, data.mDirection, " and ");
	StringUtility::appendConditionString(conditionString, MonsterFrameData::COL_ACTION, data.mAction, "");
	return doUpdate(updateString, conditionString);
}

bool SQLiteMonsterFrame::insert(const MonsterFrameData& data)
{
	string valueString;
	data.insert(valueString);
	StringUtility::removeLastComma(valueString);
	return doInsert(valueString);
}